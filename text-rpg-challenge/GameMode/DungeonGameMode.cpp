#include "DungeonGameMode.h"
#include "../Unit/Player/Player.h"
#include "../Core/User.h"
#include "../Enums/EStatType.h"
#include "../Enums/EBattleActionType.h"
#include "../Item/Potion.h"
#include "../Utils/StatFormatting.h"
#include "../Unit/Monster/Monster.h"
#include "../Data/BattleData.h"
#include "../Data/StatData.h"
#include <array>
#include <vector>
#include <string>

namespace TextRPG
{
	void DungeonGameMode::Setup(GameStateBase* state, UIManager* ui)
	{
		GameModeBase::Setup(state, ui);

		// 메인 메뉴 선택
		m_UI->OnMenuNavigationRequested.AddListener([this](int choice)
		{
			switch (choice)
			{
			case 1: m_StateMachine.ChangeState(EGameState::GS_TOWN); break;
			case 2: m_StateMachine.ChangeState(EGameState::GS_DUNGEON_EXPLORE); break;
			case 3: m_StateMachine.PushState(EGameState::GS_STAT_UPGRADE); break;
			case 4: m_StateMachine.PushState(EGameState::GS_INVENTORY); break;
			case 0: m_StateMachine.ChangeState(EGameState::GS_GAMEOVER); break;
			default:
				m_UI->PrintMessage("Invalid choice. Please try again.");
				break;
			}
		});

		// 마을 행동 선택
		m_UI->OnTownActionSelected.AddListener([this](int choice) {
			switch (choice) {
			case 1: m_UI->PromptJobChange(*m_State->GetPlayer()); break;
			case 2: m_StateMachine.PushState(EGameState::GS_SHOP); break;
			case 0: m_StateMachine.PopState(); m_UI->PrintMessage("Leaving town."); break;
			default: m_UI->PrintMessage("Invalid choice. Please try again."); break;
			}
		});

		// 상점 행동 선택
		m_UI->OnPotionShopActionSelected.AddListener([this](int choice) {
			switch (choice) {
			case 1: m_StateMachine.PushState(EGameState::GS_SHOP_BUY); break;
			case 2: m_StateMachine.PushState(EGameState::GS_SHOP_SELL); break;
			case 3: m_StateMachine.PushState(EGameState::GS_SHOP_CRAFT); break;
			case 0: m_StateMachine.PopState(); m_UI->PrintMessage("Leaving the potion shop."); break;
			default: m_UI->PrintMessage("Invalid choice. Please try again."); break;
			}
		});

		// 아이템 구매/판매 요청 처리
		m_UI->OnShopBuyRequested.AddListener([this](int choice, int count) {
			if (choice == 0) { m_StateMachine.PopState(); return; }
			_handleShopBuyLogic(choice, count);
		});
		m_UI->OnShopSellRequested.AddListener([this](int choice, int count) {
			if (choice == 0) { m_StateMachine.PopState(); return; }
			_handleShopSellLogic(choice, count);
		});
		m_UI->OnShopCraftActionSelected.AddListener([this](int choice) {
			if (choice == 0) { m_StateMachine.PopState(); return; }
			
			std::vector<Recipe> recipes;
			if (choice == 1) {
				recipes = m_PotionShop.ShowAllRecipe();
			} else if (choice == 2) {
				std::string name = m_UI->GetStringInput("Enter recipe name: ");
				recipes = m_PotionShop.SearchRecipeByName(name);
			} else if (choice == 3) {
				std::string ingredient = m_UI->GetStringInput("Enter ingredient name: ");
				recipes = m_PotionShop.SearchRecipeByIngredient(ingredient);
			} else {
				m_UI->PrintMessage("Invalid choice. Please try again.");
				return;
			}

			m_UI->DisplayRecipes(recipes, m_PotionShop.GetStock());
			if (recipes.empty()) return;

			int recipeIdx = 0;
			m_UI->GetInputs("Enter recipe number to craft (0 to cancel): ", recipeIdx);
			if (recipeIdx > 0 && recipeIdx <= recipes.size()) {
				Recipe targetRecipe = recipes[recipeIdx - 1];
				if (m_PotionShop.CraftItem(targetRecipe, m_State->GetUser().GetInventory())) {
					m_UI->PrintMessage("Successfully crafted: " + targetRecipe.GetName() + "!");
				} else {
					m_UI->PrintMessage("Crafting failed. Not enough ingredients.");
				}
			}
		});


		// 인벤토리 행동 선택
		m_UI->OnInventoryActionSelected.AddListener([this](int choice) {
			switch (choice) {
			case 1: m_StateMachine.PushState(EGameState::GS_INVENTORY_USE); break;
			case 0: m_StateMachine.PopState(); break;
			default: m_UI->PrintMessage("Invalid choice. Please try again."); break;
			}
		});

		// 스탯 강화 행동 선택
		m_UI->OnStatUpgradeMenuSelected.AddListener([this](int choice) {
			switch (choice) {
			case 1: m_UI->DisplayStatEffects(); break;
			case 2: 
				if (m_State->GetPlayer()->GetAvailableStatPoints() <= 0) m_UI->PrintMessage("You have no available stat points to spend.");
				else m_StateMachine.PushState(EGameState::GS_STAT_ALLOCATION);
				break;
			case 0: m_StateMachine.PopState(); m_UI->PrintMessage("Returning."); break;
			default: m_UI->PrintMessage("Invalid choice. Please try again."); break;
			}
		});

		// 캐릭터 이름 지정
		m_UI->OnCharacterNameRequested.AddListener([this](std::string name)
		{
			m_State->GetPlayer()->SetName(name);
		});

		// 초기 HP / MP 할당
		m_UI->OnInitialStatRequested.AddListener([this](int hp, int mp)
		{
			std::array<int, static_cast<int>(EStatType::ST_Count)> baseStats;
			baseStats.fill(0);
			baseStats[static_cast<int>(EStatType::ST_Health)] = hp;
			baseStats[static_cast<int>(EStatType::ST_Mana)] = mp;
			
			Player* player = m_State->GetPlayer();
			player->Initialize(player->GetName(), baseStats);
		});

		// 스탯 강화
		m_UI->OnStatUpgradeRequested.AddListener([this](EStatType stat, int amount) 
		{
			if (stat == EStatType::ST_Count && amount == 0) {
				if (m_StateMachine.GetCurrentState() == EGameState::GS_STAT_ALLOCATION) {
					m_StateMachine.PopState();
				} else {
					m_bExitStatUpgrade = true;
				}
				return;
			}

			Player* player = m_State->GetPlayer();
			if (player->SpendStatPoints(amount)) {
				player->SetBaseStat(stat, player->GetBaseStat(stat) + amount);
				m_UI->PrintMessage(GetStatName(stat) + " increased by " + std::to_string(amount) + ".");
				m_UI->DisplayCharacterSheet(*player);
				
				if (player->GetAvailableStatPoints() <= 0 && m_StateMachine.GetCurrentState() == EGameState::GS_STAT_ALLOCATION)
					m_StateMachine.PopState();
			}
			else {
				m_UI->PrintMessage("You don't have enough points. You only have " + std::to_string(player->GetAvailableStatPoints()) + " left.");
			}
		});

		// 직업 변경
		m_UI->OnJobChangeRequested.AddListener([this](EJobType job)
		{
			Player* player = m_State->GetPlayer();
			
			std::string bonusMessage = GetStatNameByJobType(job);
			player->ChangeJob(job);

			m_UI->PrintMessage("* You became a " + GetJobName(job) + "! (" + bonusMessage + ")");
			m_UI->DisplayCharacterSheet(*player);
		});

		// 아이템 사용
		m_UI->OnItemUseRequested.AddListener([this](int itemID)
		{
			if (itemID == -1) 
			{
				m_bItemUsedSuccessfully = false;
				if (m_StateMachine.GetCurrentState() == EGameState::GS_INVENTORY_USE) m_StateMachine.PopState();
				return;
			}
			Player* player = m_State->GetPlayer();
			
			ItemUseResult useResult = m_State->GetUser().UseItem(itemID, *player);
			m_UI->DisplayItemUseResult(useResult);
			m_bItemUsedSuccessfully = useResult.bSuccess;

			if (m_StateMachine.GetCurrentState() == EGameState::GS_INVENTORY_USE)
				m_StateMachine.PopState();
		});

		// 던전 행동 선택
		m_UI->OnDungeonRoomSelected.AddListener([this](int choice, int maxIndex)
		{
			if(choice == 0) m_StateMachine.ChangeState(EGameState::GS_MAIN_MENU);
			else if (choice > 0 && choice <= maxIndex)
			{
				Room* selectedRoom = m_CurrentFloor->GetRoom(choice);
				if(selectedRoom->bIsCleared)
				{
					m_UI->PrintMessage("This room has already been cleared.");
					return;
				}
				m_CurrentRoom = selectedRoom;
				m_StateMachine.PushState(EGameState::GS_BATTLE);
			}
			else m_UI->PrintMessage("Invalid choice. Please try again.");
		});


		// 전투 행동 선택
		m_UI->OnBattleActionSelected.AddListener([this](EBattleActionType actionType)
		{
			Player* player = m_State->GetPlayer();
			if (!CurrentMonster()) return;

			switch (actionType)
			{
				case EBattleActionType::BAT_ATTACK:
					m_UI->DisplayAttackResult(player->Attack(*CurrentMonster()));
					m_bPlayerTurnConsumed = true;
					break;
				case EBattleActionType::BAT_SKILL:
					m_UI->PrintMessage("TODO Skill (implemented)");
					break;
				case EBattleActionType::BAT_ITEM:
					m_UI->DisplayInventory(m_State->GetUser().GetInventory());
					m_bItemUsedSuccessfully = false;
					
					m_UI->PromptItemUse(m_State->GetUser().GetInventory());
					if (m_bItemUsedSuccessfully)
						m_bPlayerTurnConsumed = true;
					break;
				case EBattleActionType::BAT_RUN:
				    break;
				default:
					m_UI->PrintMessage("Invalid choice. Please try again.");
					break;
			}
		});
	}

	void DungeonGameMode::Run()
	{
		InitGame();

		m_StateMachine.ChangeState(EGameState::GS_MAIN_MENU);
		m_bGameOver = false;
		while (!m_bGameOver)
		{
			switch (m_StateMachine.GetCurrentState())
			{
			case EGameState::GS_MAIN_MENU:
				m_UI->PromptMainMenuAction(m_State->GetUser().GetInventory().GetGold());
				break;
			case EGameState::GS_TOWN:
				ProcessTown();
				break;
			case EGameState::GS_SHOP:
				ProcessPotionShop();
				break;
			case EGameState::GS_SHOP_BUY:
				m_UI->PromptShopBuyAction(m_PotionShop.GetStock(), m_State->GetUser().GetInventory().GetGold());
				break;
			case EGameState::GS_SHOP_SELL:
				m_UI->PromptShopSellAction(m_State->GetUser().GetInventory());
				break;
			case EGameState::GS_SHOP_CRAFT:
				m_UI->PromptShopCraftAction();
				break;
			case EGameState::GS_STAT_UPGRADE:
				ProcessStatsUpgrade();
				break;
			case EGameState::GS_STAT_ALLOCATION:
			{
				Player* player = m_State->GetPlayer();
				std::vector<std::string> coreStatNames;
				for (EStatType statType : UpgradeableCoreStats) coreStatNames.push_back(GetStatName(statType));
				m_UI->PromptStatAllocation(coreStatNames, player->GetBaseStats(), player->GetAvailableStatPoints());
				break;
			}
			case EGameState::GS_INVENTORY:
				ProcessInventory();
				break;
			case EGameState::GS_INVENTORY_USE:
				m_UI->PromptItemUse(m_State->GetUser().GetInventory());
				break;
			case EGameState::GS_DUNGEON_EXPLORE:
				m_UI->PromptDungeonAction(*m_CurrentFloor);
				break;
			case EGameState::GS_BATTLE:
				ProcessBattle();
				m_StateMachine.PopState();
				break;
			case EGameState::GS_GAMEOVER:
				m_bGameOver = true;
				break;
			default:
				m_StateMachine.ChangeState(EGameState::GS_MAIN_MENU);
				break;
			}
		}
	}

	void DungeonGameMode::ProcessBattle()
	{
		if (!m_CurrentRoom || !m_CurrentRoom->RoomMonster) return;

		Monster* monster = m_CurrentRoom->RoomMonster; // 몬스터 객체를 복사하는 대신 포인터를 직접 사용

		Player* player = m_State->GetPlayer();
		m_UI->PrintTitle("[ Battle Start! ] " + player->GetName() + " vs " + monster->GetName());

		bool isBattleOver = false;
		bool isPlayerTurn = true;

		while (!isBattleOver)
		{
			if (isPlayerTurn)
			{
				m_bPlayerTurnConsumed = false;
				m_UI->PromptBattleAction();
				if (!m_bPlayerTurnConsumed) continue;

				if (monster->GetCurrentHP() <= 0) // 포인터를 통해 원본 몬스터의 체력에 접근
				{
					m_UI->PrintMessage(monster->GetName() + " has been defeated!");

					//TODO 몬스터 드랍세팅 위치변경 예정
					BattleWinData winData;
					winData.EarnedExp = monster->GetDropExp();
					winData.EarnedGold = monster->GetDropGold();
					if (monster->GetDropItem() != nullptr) // ReleaseDropItem도 원본 몬스터에 대해 호출
					{
						winData.Rewards.push_back(monster->ReleaseDropItem());
					}

					m_UI->DisplayBattleWinDetails(winData);
					m_State->GetUser().GetInventory().GainGold(winData.EarnedGold);
					if (winData.Rewards.size() > 0)
					{
						for(ItemBase* item : winData.Rewards)
						{
							m_State->GetUser().GetInventory().AddItem(item);
						}
					}
					player->AddExp(winData.EarnedExp);
					isBattleOver = true;
					m_CurrentRoom->bIsCleared = true;
				}
			}
			else
			{
				m_UI->PrintMessage("");
				m_UI->PrintMessage("--- Monster Turn ---");
				m_UI->DisplayAttackResult(monster->Attack(*player)); // 포인터를 통해 원본 몬스터의 공격 함수 호출

				if(player->GetCurrentHP() <= 0)
				{
					m_UI->PrintMessage(player->GetName() + " has been defeated!");
					BattleLoseData loseData;
					m_UI->DisplayBattleLoseDetails(loseData);
					isBattleOver = true;
				}
			}
			isPlayerTurn = !isPlayerTurn;
		}
	}

	void DungeonGameMode::ProcessTown()
	{
		m_UI->PromptTownAction();
	}

	void DungeonGameMode::InitGame()
	{
		m_UI->PromptCharacterName();
		m_UI->PromptInitialStatAllocation();
		_handleBaseStatDistribution();
		m_UI->DisplayDefaultItem(_handleReceiveDefaultItem());

		// 상점 초기 재고 설정
		ItemEffects mpEffect; mpEffect.RestoreMP.Value = 50;
		m_PotionShop.AddItemToStock(new Potion(111, "Mana Potion", mpEffect, 50, 5));
		
		ItemEffects hpEffect; hpEffect.RestoreHP.Value = 100;
		m_PotionShop.AddItemToStock(new Potion(101, "Health Potion", hpEffect, 50, 10));

		// ==========================================
		// [테스트용 레시피 데이터 추가]
		Recipe hpRecipe(1,"Health Potion", 101, 1, { {100, 2} }); // 빈 포션(100) 2개로 HP포션(101) 1개 제작
		m_PotionShop.AddRecipe(hpRecipe);

		Recipe mpRecipe(2,"Mana Potion", 111, 1, { {100, 1}, {101, 1} }); // 빈포션 1개 + HP포션 1개로 MP포션(111) 1개 제작
		m_PotionShop.AddRecipe(mpRecipe);
		// ==========================================

		m_CurrentFloor->GenerateRooms(1);
	}

	void DungeonGameMode::_handleBaseStatDistribution()
	{
		m_UI->PrintGuide("Initialize Stat: Core Attributes", "Distribute a total of 20 points among the following stats.");
		Player* player = m_State->GetPlayer();
		const auto& coreStatTypes = UpgradeableCoreStats;
		std::vector<std::string> coreStatNames;
		for (EStatType statType : coreStatTypes) coreStatNames.push_back(GetStatName(statType));

		m_bExitStatUpgrade = false;
		while (player->GetAvailableStatPoints() > 0 && !m_bExitStatUpgrade)
		{
			m_UI->PromptStatAllocation(coreStatNames, player->GetBaseStats(), player->GetAvailableStatPoints());
		}
		m_UI->PrintMessage("Finished stat distribution.");
	}
	std::vector<std::pair<std::string, int>> DungeonGameMode::_handleReceiveDefaultItem()
	{
		if (!m_State) return {};

		Inventory& inventory = m_State->GetUser().GetInventory();
		std::vector<std::pair<std::string, int>> receivedItems;

		// 체력 포션 지급
		std::string hpPotionName = "Health Potion";
		int hpPotionId = 101;
		int hpPotionCount = 5;
		int hpPotionPrice = 50;
		ItemEffects effect;
		effect.RestoreHP.Type = EItemValueType::IVT_FLAT;
		effect.RestoreHP.Value = 100;

		inventory.AddItem(new Potion(hpPotionId, hpPotionName, effect, hpPotionPrice, hpPotionCount));
		receivedItems.push_back({ hpPotionName, hpPotionCount });

		// 마나 포션 지급
		std::string mpPotionName = "Mana Potion";
		int mpPotionId = 111;
		int mpPotionCount = 5;
		int mpPotionPrice = 50;
		ItemEffects effect2;
		effect2.RestoreMP.Type = EItemValueType::IVT_FLAT;
		effect2.RestoreMP.Value = 100;
		inventory.AddItem(new Potion(mpPotionId, mpPotionName, effect2, mpPotionPrice, mpPotionCount));
		receivedItems.push_back({ mpPotionName, mpPotionCount });

		// 빈 포션 지급 (테스트용)
		ItemEffects emptyEffect; 
		inventory.AddItem(new Potion(100, "Empty Potion", emptyEffect, 5, 3));
		receivedItems.push_back({ "Empty Potion", 3 });

		return receivedItems;
	}

	void DungeonGameMode::ProcessStatsUpgrade()
	{
		m_UI->PromptStatUpgradeAction(*m_State->GetPlayer());
	}


	void DungeonGameMode::ProcessInventory()
	{
		m_UI->PromptInventoryAction(m_State->GetUser().GetInventory());
	}

	void DungeonGameMode::ProcessPotionShop()
	{
		m_UI->PromptPotionShopAction();
	}

	void DungeonGameMode::_handleShopBuyLogic(int choice, int count)
	{
		Inventory& inven = m_State->GetUser().GetInventory();

		if (choice <= 0 || count <= 0) 
		{
			m_UI->PrintMessage("Invalid choice or quantity.");
			return;
		}

		const auto& stockItems = m_PotionShop.GetStock().GetAllItems();
		if (choice > stockItems.size())
		{
			m_UI->PrintMessage("Invalid choice. Please try again.");
			return;
		}

		auto it = stockItems.begin();
		std::advance(it, choice - 1);
		int itemID = it->second->GetID();

		if (itemID == 101) 
		{
			m_UI->PrintMessage("\n[Notice] HP Potions cannot be bought with gold. Use Empty Potions.");
			int exchangeCount = 0;
			m_UI->GetInputs("How many Empty Potions (ID: 100) will you exchange? (0 to cancel): ", exchangeCount);
			
			if (exchangeCount > 0)
			{
				if (m_PotionShop.ExchangeEmptyPotion(exchangeCount, inven))
					m_UI->PrintMessage("Exchanged successfully. You received " + std::to_string(exchangeCount) + " HP Potion(s).");
				else
					m_UI->PrintMessage("Exchange failed. Not enough Empty Potions.");
			}
			return;
		}

		ItemBase* item = m_PotionShop.GetStock().FindItem(itemID);
		if (!item)
		{
			m_UI->PrintMessage("Invalid Item ID.");
			return;
		}

		if (m_PotionShop.BuyItem(itemID, count, inven))
			m_UI->PrintMessage("Successfully bought " + std::to_string(count) + " " + item->GetName() + "(s).");
		else
			m_UI->PrintMessage("Not enough gold to buy " + std::to_string(count) + " " + item->GetName() + "(s).");
	}

	void DungeonGameMode::_handleShopSellLogic(int choice, int count)
	{
		Inventory& inven = m_State->GetUser().GetInventory();
		
		if (choice <= 0 || count <= 0) 
		{
			m_UI->PrintMessage("Invalid choice or quantity.");
			return;
		}

		const auto& invenItems = inven.GetAllItems();
		if (choice > invenItems.size())
		{
			m_UI->PrintMessage("Invalid choice. Please try again.");
			return;
		}

		auto it = invenItems.begin();
		std::advance(it, choice - 1);
		int itemID = it->second->GetID();

		int earned = m_PotionShop.SellItem(itemID, count, inven);
		if (earned >= 0)
			m_UI->PrintMessage("Successfully sold. You earned " + std::to_string(earned) + "G.");
		else
			m_UI->PrintMessage("Sell failed. Check the Item ID or quantity.");
	}
}
