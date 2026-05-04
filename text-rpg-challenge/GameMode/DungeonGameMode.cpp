#include "DungeonGameMode.h"
#include "../Unit/Player/Player.h"
#include "../Core/User.h"
#include "../Enums/EStatType.h"
#include "../Item/Potion.h"
#include "../Utils/StatFormatting.h"
#include "../GameState/DungeonGameState.h"
#include <array>
#include <vector>
#include <string>

namespace TextRPG
{
	void DungeonGameMode::InitGame()
	{
		ProcessCharacterCreation();
		// auto receivedItems = ProcessReceiveDefaultItem();
		m_UI->PrintDefaultItem(ProcessReceiveDefaultItem());
	}

	void DungeonGameMode::ProcessClassChange()
	{
		DungeonGameState* gameState = dynamic_cast<DungeonGameState*>(m_State);
		if (!gameState) return;
		Player* player = gameState->GetPlayer();

		if (player->GetJob() != EJobType::JT_NOVICE)
		{
			return; // 이미 전직했습니다.
		}

		EJobType selectedJob = m_UI->ShowClassChangeMenuAndGetChoice(*player);
		if (selectedJob == EJobType::JT_NOVICE)
		{
			return;
		}

		std::string bonusMessage = GetStatNameByJobType(selectedJob);
		player->ChangeJob(selectedJob);

		m_UI->PrintMessage("* You became a " + GetJobName(selectedJob) + "! (" + bonusMessage + ")");
		m_UI->PrintPlayerStats(*player);
	}

	void DungeonGameMode::Run()
	{
		InitGame();
		ProcessClassChange();

		bool isGameOver = false;
		ProcessCharacterUpgrade();
		while (!isGameOver)
		{

		}
	}

	void DungeonGameMode::ProcessCharacterCreation()
	{
		// 1. 타이틀 출력
		m_UI->PrintTitle("[ Dungeon Escape Text RPG ]");

		// 2. 플레이어 이름 입력
		std::string name = m_UI->GetStringInput("Enter your hero's name: ");

		// 3. 스탯 분배
		std::array<int, static_cast<int>(EStatType::ST_Count)> baseStats = m_UI->PerformInitialStatDistribution();

		// 4. Player 객체에 정보 설정
		DungeonGameState* gameState = dynamic_cast<DungeonGameState*>(m_State);
		Player* player = gameState->GetPlayer();
		player->Initialize(name, baseStats);

		// 5. 최종 스탯 출력
		m_UI->PrintPlayerStats(*player);
	}

	std::vector<std::pair<std::string, int>> DungeonGameMode::ProcessReceiveDefaultItem()
	{
		DungeonGameState* gameState = dynamic_cast<DungeonGameState*>(m_State);
		if (!gameState) return {};

		Inventory& inventory = gameState->GetUser().GetInventory();
		std::vector<std::pair<std::string, int>> receivedItems;

		// 체력 포션 지급
		int hpPotionId = 101;
		std::string hpPotionName = "Health Potion";
		int hpPotionCount = 5;
		inventory.AddItem(new Potion(hpPotionId, hpPotionName, EItemValueType::IVT_FLAT, 100, hpPotionCount));
		receivedItems.push_back({hpPotionName, hpPotionCount});

		// 마나 포션 지급
		int mpPotionId = 111;
		std::string mpPotionName = "Mana Potion";
		int mpPotionCount = 5;
		inventory.AddItem(new Potion(mpPotionId, mpPotionName, EItemValueType::IVT_FLAT, 50, mpPotionCount));
		receivedItems.push_back({mpPotionName, mpPotionCount});

		return receivedItems;
	}

	void DungeonGameMode::ProcessCharacterUpgrade()
	{
		DungeonGameState* gameState = dynamic_cast<DungeonGameState*>(m_State);
		if (!gameState) return;

		Player* player = gameState->GetPlayer();

		while (true)
		{
			m_UI->PrintUpgradeMenu(*player);
			int choice = 0;
			m_UI->GetInputs("Enter your choice: ", choice);

			switch (choice)
			{
			case 1:
				m_UI->PrintStatEffects();
				break;
			case 2:
				if (player->GetAvailableStatPoints() <= 0)
				{
					m_UI->PrintMessage("You have no available stat points to spend.");
					break;
				}
				_handleDistributeBaseStats(player);
				break;
			case 0:
				m_UI->PrintMessage("Exiting upgrade menu.");
				return;
			default:
				m_UI->PrintMessage("Invalid choice. Please try again.");
				break;
			}
		}
	}

	void DungeonGameMode::_handleDistributeBaseStats(Player* player)
	{
		m_UI->ShowStatDistribution(
			[player]() { return player->GetAvailableStatPoints(); },
			[player](int amount) { return player->SpendStatPoints(amount); },
			[player]() -> const auto& { return player->GetBaseStats(); },
			[player](EStatType stat, int amount) {
				player->SetBaseStat(stat, player->GetBaseStat(stat) + amount);
			},
			[this, player]() {
				m_UI->PrintPlayerStats(*player);
			}
		);
	}
}
