#pragma once
#include "GameModeBase.h"
#include "../Core/StateMachine.h"
#include "../GameManager/UIManager.h"
#include "../Shop/PotionShop.h"
#include "../Enums/EGameState.h"
#include <functional>

namespace TextRPG
{
	class Monster;

	class DungeonGameMode : public GameModeBase
	{
	public:
		DungeonGameMode() = default;

		virtual void Setup(GameStateBase* state, UIManager* ui) override;

		/** @brief 캐릭터 스탯 강화 과정을 시작합니다. */
		void ProcessStatsUpgrade();

		/** @brief 전투 과정을 처리합니다. */
		void ProcessBattle();

		/** @brief 마을에서의 활동을 처리합니다.*/
		void ProcessTown();

		void ProcessInventory();
		void ProcessPotionShop();
		void ProcessDungeonExplore();

		virtual void Run() override;
		virtual void InitGame() override;

	private:
		bool m_bExitStatUpgrade = false; // 루프 탈출 조건
		bool m_bItemUsedSuccessfully = false; // 아이템 사용 성공 여부
		bool m_bPlayerTurnConsumed = false; // 전투 턴 소모 여부
		bool m_bGameOver = false; // 게임 종료 여부
		Monster* m_CurrentMonster = nullptr; // 현재 교전 중인 몬스터
		PotionShop m_PotionShop; // 포션 상점 객체
		StateMachine<EGameState> m_StateMachine; // 상태 머신 관리자

		/** @brief 기본 스탯 분배 과정을 처리합니다. */
		void _handleBaseStatDistribution();
		
		/**
		 * @brief 캐릭터에게 기본 아이템을 지급하고, 지급된 아이템 목록을 반환합니다.
		 * @return 지급된 아이템의 이름과 개수 목록
		 */
		std::vector<std::pair<std::string, int>> _handleReceiveDefaultItem();

		void _handleShopBuyLogic(int choice, int count);
		void _handleShopSellLogic(int choice, int count);

	};
}