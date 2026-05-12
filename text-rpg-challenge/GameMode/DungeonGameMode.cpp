#include "DungeonGameMode.h"
#include "../Unit/Player/Player.h"
#include "../Core/User.h"
#include "../Enums/EStatType.h"
#include "../Item/Potion.h"
#include "../Data/StatData.h"
#include <array>
#include <string>

namespace TextRPG
{
	void DungeonGameMode::Setup(GameStateBase* state, UIManager* ui)
	{
		GameModeBase::Setup(state, ui);

		m_BattleController.Setup(m_UI, GetDungeonState(), &m_StateMachine);
		m_TownController.Setup(m_UI, GetDungeonState(), &m_StateMachine);
		m_JobController.Setup(m_UI, GetDungeonState(), &m_StateMachine);
		m_StatController.Setup(m_UI, GetDungeonState(), &m_StateMachine);
		m_DungeonController.Setup(m_UI, GetDungeonState(), &m_StateMachine);
		m_InventoryController.Setup(m_UI, GetDungeonState(), &m_StateMachine);
		m_ShopController.Setup(m_UI, &m_PotionShop, &m_State->GetUser(), &m_StateMachine);

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
		
		m_UI->OnCharacterNameRequested.AddListener([this](std::string name) { m_State->GetPlayer()->SetName(name); });

		// 초기 HP / MP 할당
		m_UI->OnInitialStatRequested.AddListener([this](int hp, int mp) {
			std::array<int, static_cast<int>(EStatType::ST_Count)> baseStats; baseStats.fill(0);
			baseStats[static_cast<int>(EStatType::ST_Health)] = hp; baseStats[static_cast<int>(EStatType::ST_Mana)] = mp;
			m_State->GetPlayer()->Initialize(m_State->GetPlayer()->GetName(), baseStats);
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
				m_UI->PromptTownAction(); 
				break;
			case EGameState::GS_SHOP:
				m_ShopController.Process();
				break;
			case EGameState::GS_STAT_UPGRADE:
			case EGameState::GS_STAT_ALLOCATION:
				m_StatController.Process();
				break;
			case EGameState::GS_JOB_CENTER:
				m_JobController.Process();
				break;
			case EGameState::GS_INVENTORY:
			case EGameState::GS_INVENTORY_USE:
				m_InventoryController.Process();
				break;
			case EGameState::GS_DUNGEON_EXPLORE:
				m_DungeonController.Process(); 
				break;
			case EGameState::GS_BATTLE:
				m_BattleController.Process();
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

		GetDungeonState()->GetCurrentFloor()->GenerateRooms(1);
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

}