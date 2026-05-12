#pragma once
#include "../GameManager/UIManager.h"
#include "../GameState/DungeonGameState.h"
#include "../Core/StateMachine.h"
#include "../Enums/EGameState.h"
#include "../Enums/EBattleActionType.h"
#include "../Core/User.h"

namespace TextRPG
{
	class BattleController
	{
	private:
		UIManager* m_UI = nullptr;
		DungeonGameState* m_DungeonGameState = nullptr;
		StateMachine<EGameState>* m_GlobalStateMachine = nullptr;

		bool m_bPlayerTurnConsumed = false;
		bool m_bItemUsedSuccessfully = false;

	public:
		BattleController() = default;

		/**
		 * @brief BattleController를 초기화하고 필요한 의존성을 주입합니다.
		 * @param ui UIManager 객체 포인터
		 * @param dungeonGameState DungeonGameState 객체 포인터
		 * @param globalSM 전역 상태 머신 포인터
		 */
		void Setup(UIManager* ui, DungeonGameState* dungeonGameState, StateMachine<EGameState>* globalSM);

		/** @brief 전투 컨트롤러의 메인 처리 로직을 실행합니다. */
		void Process();

	private:
		/**
		 * @brief 플레이어의 전투 행동 선택 이벤트를 처리합니다.
		 * @param actionType 선택된 전투 행동 타입
		 */
		void _handleBattleAction(EBattleActionType actionType);

		/** @brief 아이템 사용 결과를 처리합니다. */
		void _handleItemUseResult(int itemID);
	};
}
