#pragma once
#include "../GameManager/UIManager.h"
#include "../GameState/DungeonGameState.h"
#include "../Core/StateMachine.h"
#include "../Enums/EGameState.h"
#include "../Enums/EJobType.h"

namespace TextRPG
{
	class JobController
	{
	private:
		UIManager* m_UI = nullptr;
		DungeonGameState* m_DungeonGameState = nullptr;
		StateMachine<EGameState>* m_GlobalStateMachine = nullptr;

	public:
		JobController() = default;

		/**
		 * @brief JobController를 초기화하고 필요한 의존성을 주입합니다.
		 * @param ui UIManager 객체 포인터
		 * @param dungeonGameState DungeonGameState 객체 포인터 (User 및 Player 접근용)
		 * @param globalSM 전역 상태 머신 포인터
		 */
		void Setup(UIManager* ui, DungeonGameState* dungeonGameState, StateMachine<EGameState>* globalSM);

		/** @brief 직업 컨트롤러의 메인 처리 로직을 실행합니다. */
		void Process();

	private:
		/**
		 * @brief 직업 변경 요청 이벤트를 처리합니다.
		 * @param job 변경할 직업 타입
		 */
		void _handleJobChangeRequest(int choice); // Changed to int
	};
}