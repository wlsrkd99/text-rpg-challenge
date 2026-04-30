#pragma once

#include "GameStateBase.h"
#include "../Unit/Player/Player.h"

namespace TextRPG
{
	namespace GameState
	{
		class DungeonGameState : public GameStateBase
		{
		private:
			Unit::Player m_MainPlayer; // Player 객체를 직접 소유
		public:
			virtual void SaveData();
			virtual void LoadData();

			/** @brief 게임 상태 내의 메인 플레이어 객체 포인터를 반환합니다. */
			Unit::Player* GetPlayer();
		};
	}
}
