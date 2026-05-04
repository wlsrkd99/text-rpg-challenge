#pragma once

#include "GameModeBase.h"
#include "../GameManager/UIManager.h"
#include "../GameState/DungeonGameState.h"
#include <functional>

namespace TextRPG
{
	class DungeonGameMode : public GameModeBase
	{
	public:
		DungeonGameMode() = default;

		/** @brief 캐릭터 생성 과정을 시작합니다. */
		void ProcessCharacterCreation();

		/** @brief 플레이어의 직업 변경 과정을 처리합니다. */
		void ProcessClassChange();

		/**
		 * @brief 캐릭터에게 기본 아이템을 지급하고, 지급된 아이템 목록을 반환합니다.
		 * @return 지급된 아이템의 이름과 개수 목록
		 */
		std::vector<std::pair<std::string, int>> ProcessReceiveDefaultItem();

		/** @brief 캐릭터 스탯 강화 과정을 시작합니다. */
		void ProcessCharacterUpgrade();

		virtual void InitGame();
		virtual void Run();

	private:
		/** @brief 플레이어의 기본 스탯 분배 과정을 처리합니다. */
		void _handleDistributeBaseStats(Player* player);

	};
}