#pragma once
#include "../GameManager/UIManager.h"
#include "../GameState/DungeonGameState.h"
#include "../Core/StateMachine.h"
#include "../Enums/EGameState.h"
#include "../Enums/EStatType.h"

namespace TextRPG
{
	class StatController
	{
	private:
		UIManager* m_UI = nullptr;
		DungeonGameState* m_DungeonGameState = nullptr;
		StateMachine<EGameState>* m_GlobalStateMachine = nullptr;

	public:
		StatController() = default;

		/**
		 * @brief StatController를 초기화하고 필요한 의존성을 주입합니다.
		 * @param ui UIManager 객체 포인터
		 * @param dungeonGameState DungeonGameState 객체 포인터 (User 및 Player 접근용)
		 * @param globalSM 전역 상태 머신 포인터
		 */
		void Setup(UIManager* ui, DungeonGameState* dungeonGameState, StateMachine<EGameState>* globalSM);

		/** @brief 스탯 컨트롤러의 메인 처리 로직을 실행합니다. */
		void Process();

	private:
		/**
		 * @brief 스탯 강화 메뉴 선택 이벤트를 처리합니다.
		 * @param choice 선택된 메뉴 항목 (1: 스탯 효과 보기, 2: 스탯 분배, 0: 뒤로가기)
		 */
		void _handleStatUpgradeMenuSelection(int choice);

		/** @brief 스탯 강화 요청 이벤트를 처리합니다. */
		void _handleStatUpgradeRequest(EStatType stat, int amount);
	};
}
