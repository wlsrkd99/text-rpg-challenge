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

		// 메뉴 선택
		m_UI->OnMenuNavigationRequested.AddListener([this](int choice)
		{
			switch (choice)
			{
			case 1:
				ProcessTown();
				break;
			case 2:
				ProcessBattle();
				break;
			case 3:
				ProcessStatsUpgrade();
				break;
			case 4:
				ProcessInventory();
				break;
			case 0:
				m_bGameOver = true;
				break;
			default:
				m_UI->PrintMessage("Invalid choice. Please try again.");
				break;
			}
		});

		// 캐릭터 이름 지정
		m_UI->OnCharacterNameEntered.AddListener([this](std::string name)
		{
			m_State->GetPlayer()->SetName(name);
		});

		// 초기 HP / MP 할당
		m_UI->OnInitialStatAllocated.AddListener([this](int hp, int mp)
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
				m_bExitStatUpgrade = true;
				return;
			}

			Player* player = m_State->GetPlayer();
			if (player->SpendStatPoints(amount)) {
				player->SetBaseStat(stat, player->GetBaseStat(stat) + amount);
				m_UI->PrintMessage(GetStatName(stat) + " increased by " + std::to_string(amount) + ".");
				m_UI->DisplayCharacterSheet(*player);
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
				return;
			}
			Player* player = m_State->GetPlayer();
			
			ItemUseResult useResult = m_State->GetUser().UseItem(itemID, *player);
			m_UI->DisplayItemUseResult(useResult);
			m_bItemUsedSuccessfully = useResult.bSuccess;
		});

		m_UI->OnBattleActionSelected.AddListener([this](EBattleActionType actionType)
		{
			Player* player = m_State->GetPlayer();
			if (!m_CurrentMonster) return;

			switch (actionType)
			{
				case EBattleActionType::BAT_ATTACK:
					m_UI->DisplayAttackResult(player->Attack(*m_CurrentMonster));
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

		m_bGameOver = false;
		while (!m_bGameOver)
		{
			m_UI->PromptMainMenuAction(m_State->GetUser().GetGold());
		}
	}

	void DungeonGameMode::ProcessBattle()
	{
		// 1. 전투 설정
		Monster slime;
		std::array<int, static_cast<int>(EStatType::ST_Count)> slimeStats;
		slimeStats.fill(5);
		slimeStats[static_cast<int>(EStatType::ST_Health)] = 50;
		slime.Initialize("Slime", slimeStats);
		slime.SetDropExp(50);
		slime.SetDropGold(15);
		slime.SetDropItem(new ItemBase(1, "Slime Jelly", EItemType::IT_NONE, EItemGrade::IG_COMMON, 30, 1));
		m_CurrentMonster = &slime;

		Player* player = m_State->GetPlayer();
		m_UI->PrintTitle("[ Battle Start! ] " + player->GetName() + " vs " + slime.GetName());

		bool isBattleOver = false;
		bool isPlayerTurn = true;

		while (!isBattleOver)
		{
			if (isPlayerTurn)
			{
				m_bPlayerTurnConsumed = false;
				m_UI->PromptBattleAction();
				if (!m_bPlayerTurnConsumed) continue;

				if (slime.GetCurrentHP() <= 0)
				{
					m_UI->PrintMessage(slime.GetName() + " has been defeated!");

					//TODO 몬스터 드랍세팅 위치변경 예정
					BattleWinData winData;
					winData.EarnedExp = slime.GetDropExp();
					winData.EarnedGold = slime.GetDropGold();
					if (slime.GetDropItem() != nullptr)
					{
						winData.Rewards.push_back(slime.GetDropItem());
					}

					m_UI->DisplayBattleWinDetails(winData);
					m_State->GetUser().AddGold(winData.EarnedGold);
					if (winData.Rewards.size() > 0)
					{
						for(ItemBase* item : winData.Rewards)
						{
							m_State->GetUser().GetInventory().AddItem(item);
						}
					}
					player->AddExp(winData.EarnedExp);
					isBattleOver = true;
				}
			}
			else
			{
				m_UI->PrintMessage("");
				m_UI->PrintMessage("--- Monster Turn ---");
				m_UI->DisplayAttackResult(slime.Attack(*player));

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
		m_CurrentMonster = nullptr;
	}

	void DungeonGameMode::ProcessTown()
	{
		while (true)
		{
			m_UI->PrintTitle("[ Town ]");

			MenuConfig config;
			config.Title = "You are in town. What would you like to do?";
			config.Options = {
				"1. Job Change Center",
				"2. Visit Shop (Not implemented)",
				"0. Leave Town"
			};

			int choice = m_UI->PromptMenu(config);

			switch (choice)
			{
			case 1:
				m_UI->PromptJobChange(*m_State->GetPlayer());
				break;
			case 2:
				m_UI->PrintMessage("Shop is not yet open.");
				break;
			case 0:
				m_UI->PrintMessage("Leaving town.");
				return;
			default:
				m_UI->PrintMessage("Invalid choice. Please try again.");
				break;
			}
		}
	}

	void DungeonGameMode::InitGame()
	{
		m_UI->PromptCharacterName();
		m_UI->PromptInitialStatAllocation();
		_handleBaseStatDistribution();
		m_UI->DisplayDefaultItem(_handleReceiveDefaultItem());
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

		return receivedItems;
	}

	void DungeonGameMode::ProcessStatsUpgrade()
	{
		if (!m_State) return;
		Player* player = m_State->GetPlayer();

		m_UI->DisplayCharacterSheet(*player);
		while (true)
		{
			MenuConfig config;
			config.Title = "Upgrade Actions";
			config.Infos = { "Available Stat Points: " + std::to_string(player->GetAvailableStatPoints()) };
			config.Options = { "1. Show Stat Effects", "2. Upgrade Base Stats", "0. Back to Town" };
			config.Prompt = "Enter your choice: ";

			int choice = m_UI->PromptMenu(config);
			switch (choice)
			{
			case 1:
				m_UI->DisplayStatEffects();
				break;
			case 2:
				if (player->GetAvailableStatPoints() <= 0)
				{
					m_UI->PrintMessage("You have no available stat points to spend.");
					m_UI->DisplayCharacterSheet(*player);
					break;
				}
				_handleBaseStatDistribution();
				m_UI->DisplayCharacterSheet(*player);
				break;
			case 0:
				m_UI->PrintMessage("Returning to town.");
				return;
			default:
				m_UI->PrintMessage("Invalid choice. Please try again.");
				break;
			}
		}
	}


	void DungeonGameMode::ProcessInventory()
	{
		Player* player = m_State->GetPlayer();
		User& user = m_State->GetUser();

		m_UI->DisplayInventory(user.GetInventory());

		MenuConfig config;
		config.Title = "Inventory Actions";
		config.Options = { "1. Use an item", "0. Back to Town" };
		int choice = m_UI->PromptMenu(config);

		if (choice == 1)
		{
			m_UI->PromptItemUse(user.GetInventory());
		}
	}
}
