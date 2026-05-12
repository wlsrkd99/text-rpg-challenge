#pragma once
#include "../GameManager/UIManager.h"
#include "../GameState/DungeonGameState.h"
#include "../Core/StateMachine.h"
#include "../Enums/EGameState.h"

namespace TextRPG
{
	class InventoryController
	{
	private:
		UIManager* m_UI = nullptr;
		DungeonGameState* m_DungeonGameState = nullptr;
		StateMachine<EGameState>* m_GlobalStateMachine = nullptr;

	public:
		InventoryController() = default;

		/**
		 * @brief InventoryController를 초기화하고 필요한 의존성을 주입합니다.
		 * @param ui UIManager 객체 포인터
		 * @param dungeonGameState DungeonGameState 객체 포인터 (User 및 Player 접근용)
		 * @param globalSM 전역 상태 머신 포인터
		 */
		void Setup(UIManager* ui, DungeonGameState* dungeonGameState, StateMachine<EGameState>* globalSM);

		/** @brief 인벤토리 컨트롤러의 메인 처리 로직을 실행합니다. */
		void Process();

	private:
		/**
		 * @brief 인벤토리 메인 행동 선택 이벤트를 처리합니다.
		 * @param choice 선택된 행동 (1: 아이템 사용, 0: 뒤로가기)
		 */
		void _handleInventoryAction(int choice);

		/** @brief 인벤토리에서 아이템 사용 요청 이벤트를 처리합니다. */
		void _handleItemUse(int itemID);
	};
}