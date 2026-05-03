#pragma once

#include "GameModeBase.h"
#include "../GameManager/UIManager.h"
#include "../GameState/DungeonGameState.h"


namespace TextRPG
{
	class DungeonGameMode : public GameModeBase
	{
	public:
		DungeonGameMode() = default;

		/** @brief 캐릭터 생성 과정을 시작합니다. */
		void ProcessCharacterCreation();

		/** @brief 캐릭터에게 기본 아이템을 지급합니다. */
		void ProcessReceiveDefaultItem();

		virtual void InitGame();
		virtual void Run();
	};
}