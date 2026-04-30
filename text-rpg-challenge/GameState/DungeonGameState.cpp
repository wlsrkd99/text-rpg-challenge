#include "DungeonGameState.h"

namespace TextRPG
{
	namespace GameState
	{
		Unit::Player* DungeonGameState::GetPlayer()
		{
			return &m_MainPlayer;
		}

		void DungeonGameState::SaveData()
		{
			// TODO: 데이터 저장 로직 구현
		}

		void DungeonGameState::LoadData()
		{
			// TODO: 데이터 로드 로직 구현
		}
	}
}
