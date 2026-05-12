#pragma once
#include "../GameManager/UIManager.h"
#include "../GameState/DungeonGameState.h"
#include "../Core/StateMachine.h"
#include "../Enums/EGameState.h"

namespace TextRPG
{
	class TownController
	{
	private:
		UIManager* m_UI = nullptr;
		DungeonGameState* m_DungeonGameState = nullptr;
		StateMachine<EGameState>* m_GlobalStateMachine = nullptr;

	public:
		TownController() = default;

		/**
		 * @brief TownController를 초기화하고 필요한 의존성을 주입합니다.
		 * @param ui UIManager 객체 포인터
		 * @param dungeonGameState DungeonGameState 객체 포인터 (User 및 Player 접근용)
		 * @param globalSM 전역 상태 머신 포인터
		 */
		void Setup(UIManager* ui, DungeonGameState* dungeonGameState, StateMachine<EGameState>* globalSM);

		/** @brief 마을 컨트롤러의 메인 처리 로직을 실행합니다. */
		void Process();

	private:
		/**
		 * @brief 마을에서의 행동 선택 이벤트를 처리합니다.
		 * @param choice 선택된 메뉴 항목 (1: 전직소, 2: 상점, 0: 마을 나가기)
		 */
		void _handleTownAction(int choice);
	};
}
