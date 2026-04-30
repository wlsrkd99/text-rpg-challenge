#pragma once

#include "GameModeBase.h"
#include "../GameManager/UIManager.h"
#include "../GameState/DungeonGameState.h"


namespace TextRPG
{
	namespace GameMode
	{
		class DungeonGameMode : public GameModeBase
		{
		public:
			DungeonGameMode() = default;

			/** @brief 캐릭터 생성 과정을 시작합니다. */
			void ProcessCharacterCreation();

			virtual void InitGame();
			virtual void Run();

		};
	}
}