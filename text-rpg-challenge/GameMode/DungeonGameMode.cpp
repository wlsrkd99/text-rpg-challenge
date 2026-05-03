#include "DungeonGameMode.h"
#include "../Unit/Player/Player.h"
#include "../GameState/DungeonGameState.h"
#include <array>
#include <algorithm>
#include <vector>
#include <string>
#include <numeric>

namespace TextRPG
{
	void DungeonGameMode::InitGame()
	{
		ProcessCharacterCreation();
	}

	void DungeonGameMode::Run()
	{
		InitGame();

		bool isGameOver = false;
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
				if (baseStats[static_cast<int>(EStatType::ST_Health)] > 0 && baseStats[static_cast<int>(EStatType::ST_Mana)] > 0) {
					break;
				}
				m_UI->PrintMessage("Health and Mana must be positive values. Please try again.");
			} else {
				m_UI->PrintMessage("The sum of Health and Mana must be exactly 100. Please try again.");
			}
		}

		// 4. 나머지 5개 스탯 분배 루프 (총합 20)
		m_UI->PrintMessage("\n--- Stat Distribution: Core Attributes ---");
		m_UI->PrintMessage("Distribute a total of 20 points among the following stats.");
		const std::vector<EStatType> otherStatTypes = { EStatType::ST_Endurance, EStatType::ST_Strength, EStatType::ST_Agility, EStatType::ST_Intelligence, EStatType::ST_Wisdom };
		const std::vector<std::string> otherStatNames = { "Endurance", "Strength", "Agility", "Intelligence", "Wisdom" };
		int remainingPoints = 20;

		while (remainingPoints > 0)
		{
			m_UI->PrintMessage("\nPoints left: " + std::to_string(remainingPoints));
			m_UI->PrintMessage("------------------------------------");
			for (size_t i = 0; i < otherStatTypes.size(); ++i)
			{
				m_UI->PrintMessage(std::to_string(i + 1) + ". " + otherStatNames[i] + ": " + std::to_string(baseStats[static_cast<int>(otherStatTypes[i])]));
			}
			m_UI->PrintMessage("------------------------------------");

			int choice = 0;
			m_UI->GetInputs("Choose a stat to upgrade (1-5): ", choice);

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
			if (amount > remainingPoints) {
				m_UI->PrintMessage("You don't have enough points. You only have " + std::to_string(remainingPoints) + " left.");
				continue;
			}

			baseStats[static_cast<int>(otherStatTypes[choice - 1])] += amount;
			remainingPoints -= amount;
		}

		m_UI->PrintMessage("\nAll stat points have been distributed!");
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

	}
}
