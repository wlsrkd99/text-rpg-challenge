#pragma once
#include "../GameManager/UIManager.h"
#include "../GameState/DungeonGameState.h"
#include "../Core/StateMachine.h"
#include "../Enums/EGameState.h"

namespace TextRPG
{
	class DungeonController
	{
	private:
		UIManager* m_UI = nullptr;
		DungeonGameState* m_DungeonGameState = nullptr; // 던전 상태 데이터 접근용
		StateMachine<EGameState>* m_GlobalStateMachine = nullptr; // 전역 상태 머신

	public:
		DungeonController() = default;

		/**
		 * @brief DungeonController를 초기화하고 필요한 의존성을 주입합니다.
		 * @param ui UIManager 객체 포인터
		 * @param dungeonGameState DungeonGameState 객체 포인터
		 * @param globalSM 전역 상태 머신 포인터
		 */
		void Setup(UIManager* ui, DungeonGameState* dungeonGameState, StateMachine<EGameState>* globalSM);

		/** @brief 던전 컨트롤러의 메인 처리 로직을 실행합니다. */
		void Process();

	private:
		/**
		 * @brief 던전 방 선택 이벤트를 처리합니다.
		 * @param choice 사용자가 선택한 방 번호 (0은 던전 나가기)
		 * @param maxIndex 현재 층의 최대 방 개수
		 */
		void _handleDungeonRoomSelection(int choice, int maxIndex);
	};
}