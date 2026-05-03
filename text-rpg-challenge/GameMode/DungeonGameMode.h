#pragma once

#include "GameModeBase.h"
#include "../GameManager/UIManager.h"
#include "../GameState/DungeonGameState.h"
#include <functional>

namespace TextRPG
{
	class DungeonGameMode : public GameModeBase
	{
	private:
		/** @brief 플레이어의 기본 스탯 분배 과정을 처리합니다. */
		void ProcessBaseStatDistribution(Player* player);

		/** @brief 기본 스탯 분배 로직을 처리하는 헬퍼 함수 */
		void DistributeCoreStats(
			std::function<int()> getPoints,
			std::function<bool(int)> spendPoints,
			std::function<const std::array<int, static_cast<int>(EStatType::ST_Count)>&()> getStatsArray,
			std::function<void(EStatType, int)> applyStatChange,
			std::function<void()> onUpdate = nullptr
		);
		
	public:
		DungeonGameMode() = default;

		/** @brief 캐릭터 생성 과정을 시작합니다. */
		void ProcessCharacterCreation();

		/** @brief 캐릭터에게 기본 아이템을 지급합니다. */
		void ProcessReceiveDefaultItem();

		/** @brief 캐릭터 스탯 강화 과정을 시작합니다. */
		void ProcessCharacterUpgrade();

		virtual void InitGame();
		virtual void Run();

	};
}