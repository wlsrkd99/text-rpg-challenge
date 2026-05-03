#include "DungeonGameMode.h"
#include "../Unit/Player/Player.h"
#include "../Core/User.h"
#include "../Data/EStatType.h" // For EStatType in ProcessCharacterUpgrade
#include "../Item/Potion.h"
#include "../GameState/DungeonGameState.h"
#include <array>
#include <vector>
#include <string>
#include <numeric>

namespace TextRPG
{
	void DungeonGameMode::InitGame()
	{
		ProcessCharacterCreation();
		ProcessReceiveDefaultItem();
	}

	void DungeonGameMode::Run()
	{
		InitGame();

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

		std::array<int, static_cast<int>(EStatType::ST_Count)> baseStats;
		baseStats.fill(0);

		// 3. Health & Mana 입력 루프 (총합 100)
		m_UI->PrintMessage("\n--- Stat Distribution: Health & Mana ---");
		m_UI->PrintMessage("Distribute a total of 100 points between Health and Mana.");
		while (true) 
		{
			m_UI->GetInputs("Enter Base Stats (Health Mana): ", 
				baseStats[static_cast<int>(EStatType::ST_Health)],
				baseStats[static_cast<int>(EStatType::ST_Mana)]);

			if (baseStats[static_cast<int>(EStatType::ST_Health)] + baseStats[static_cast<int>(EStatType::ST_Mana)] == 100) {
				if (baseStats[static_cast<int>(EStatType::ST_Health)] > 10 && baseStats[static_cast<int>(EStatType::ST_Mana)] > 10) {
					break;
				}
				m_UI->PrintMessage("Health and Mana must be greater than 10. Please try again.");
			} else {
				m_UI->PrintMessage("The sum of Health and Mana must be exactly 100. Please try again.");
			}
		}

		// 4. 나머지 5개 스탯 분배 루프
		m_UI->PrintMessage("\n--- Stat Distribution: Core Attributes ---");
		m_UI->PrintMessage("Distribute a total of 20 points among the following stats.");
		const std::vector<EStatType> otherStatTypes = { EStatType::ST_Endurance, EStatType::ST_Strength, EStatType::ST_Agility, EStatType::ST_Intelligence, EStatType::ST_Wisdom };
		const std::vector<std::string> otherStatNames = { "Endurance", "Strength", "Agility", "Intelligence", "Wisdom" };
	
		int remainingPoints = 20;
		DistributeCoreStats(
			[&]() { return remainingPoints; },
			[&](int amount) {
				if (amount > remainingPoints) return false;
				remainingPoints -= amount;
				return true;
			},
			[&]() -> const auto& { return baseStats; },
			[&](EStatType stat, int amount) {
				baseStats[static_cast<int>(stat)] += amount;
			},
			nullptr
		);

		m_UI->PrintMessage("\nAll initial stat points have been distributed!");
		m_UI->PrintMessage("");

		// 5. 새로운 Initialize 함수를 사용하여 Player 객체에 정보 설정
		DungeonGameState* gameState = dynamic_cast<DungeonGameState*>(m_State);
		Player* player = gameState->GetPlayer();
		player->Initialize(name, baseStats);

		// 6. 최종 스탯 출력
		m_UI->PrintPlayerStats(*player);
	}

	void DungeonGameMode::ProcessReceiveDefaultItem()
	{
		DungeonGameState* gameState = dynamic_cast<DungeonGameState*>(m_State);
		if (!gameState) return;

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

		// 획득한 아이템 정보 출력
		m_UI->PrintDefaultItem(receivedItems);
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
				ProcessBaseStatDistribution(player);
				break;
			case 0: // Exit
				m_UI->PrintMessage("Exiting upgrade menu.");
				return;
			default:
				m_UI->PrintMessage("Invalid choice. Please try again.");
				break;
			}
		}
	}

	void DungeonGameMode::ProcessBaseStatDistribution(Player* player)
	{
		DistributeCoreStats(
			// getPoints: Player 객체에서 사용 가능 포인트를 가져옵니다.
			[player]() { return player->GetAvailableStatPoints(); },
			// spendPoints: Player 객체의 SpendStatPoints 함수를 호출합니다.
			[player](int amount) { return player->SpendStatPoints(amount); },
			// getStatsArray: Player 객체에서 현재 스탯 배열을 가져옵니다.
			[player]() -> const auto& { return player->GetBaseStats(); },
			// applyStatChange: Player 객체의 SetBaseStat 함수를 호출하여 스탯을 적용합니다.
			[player](EStatType stat, int amount) {
				player->SetBaseStat(stat, player->GetBaseStat(stat) + amount);
			},
			// onUpdate: 스탯이 변경될 때마다 전체 스탯을 다시 출력합니다.
			[this, player]() {
				m_UI->PrintPlayerStats(*player);
			}
		);
	}

	void DungeonGameMode::DistributeCoreStats(
		std::function<int()> getPoints,
		std::function<bool(int)> spendPoints,
		std::function<const std::array<int, static_cast<int>(EStatType::ST_Count)>&()> getStatsArray,
		std::function<void(EStatType, int)> applyStatChange,
		std::function<void()> onUpdate)
	{
		const std::vector<EStatType> coreStatTypes = { EStatType::ST_Endurance, EStatType::ST_Strength, EStatType::ST_Agility, EStatType::ST_Intelligence, EStatType::ST_Wisdom };
		const std::vector<std::string> coreStatNames = { "Endurance", "Strength", "Agility", "Intelligence", "Wisdom" };

		while (getPoints() > 0)
		{
			m_UI->PrintStatDistributionUI(coreStatNames, getStatsArray(), getPoints());

			int choice = 0;
			m_UI->GetInputs("Choose a stat to upgrade (1-5, or 0 to exit): ", choice);
			if (choice == 0) break;
			if (choice < 1 || choice > 5) {
				m_UI->PrintMessage("Invalid choice. Please select a number between 1 and 5.");
				continue;
			}

			int amount = 0;
			m_UI->GetInputs("How many points to add?: ", amount);
			if (amount <= 0) {
				m_UI->PrintMessage("You must add a positive number of points.");
				continue;
			}

			if (!spendPoints(amount)) {
				m_UI->PrintMessage("You don't have enough points. You only have " + std::to_string(getPoints()) + " left.");
				continue;
			}

			EStatType statToUpgrade = coreStatTypes[choice - 1];
			applyStatChange(statToUpgrade, amount);

			m_UI->PrintMessage(coreStatNames[choice - 1] + " increased by " + std::to_string(amount) + ".");

			if (onUpdate) {
				onUpdate();
			}
		}
		m_UI->PrintMessage("\nFinished stat distribution.");
	}
}
