#include "DungeonGameMode.h"

namespace TextRPG
{
	void DungeonGameMode::ProcessCharacterCreation()
	{
		// 1. 타이틀 출력
		m_UI->PrintTitle("[ Dungeon Escape Text RPG ]");

		// 2. 플레이어 이름 입력
		std::string name = m_UI->GetStringInput("Enter your hero's name: ");

		// 3. 스탯 입력 (유효성 검사 루프)

		const int SIZE = 4;
		int stat[SIZE] = { 0 };	// 0: hp, 1: mp, 2: attack, 3: defense

		while (true) {
			m_UI->GetTwoIntsInput("Enter HP and MP: ", stat[0], stat[1]);
			if (stat[static_cast<int>(EStatType::Hp)] >= 50 && stat[static_cast<int>(EStatType::Mp)] >= 50) {
				break;
			}
			m_UI->PrintMessage("HP or MP is too low. Try again.");
		}

		while (true) {
			m_UI->GetTwoIntsInput("Enter Attack and Defense: ", stat[2], stat[3]);
			if (stat[static_cast<int>(EStatType::Atk)] >= 10 && stat[static_cast<int>(EStatType::Def)] >= 10) {
				break;
			}
			m_UI->PrintMessage("Attack or Defense is too low. Try again.");
		}
		m_UI->PrintMessage("");

		// 4. GameState를 통해 Player 객체에 정보 설정
		DungeonGameState* gameState = dynamic_cast<DungeonGameState*>(m_State);
		Player* player = gameState->GetPlayer();
		player->SetInfo(name, stat);

		// 5. 최종 스탯 출력
		m_UI->PrintPlayerStats(*player);
	}

	void DungeonGameMode::ProcessReceiveDefaultItem()
	{

	}

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
}
