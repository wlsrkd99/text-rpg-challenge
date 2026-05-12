#include "TownController.h"

namespace TextRPG
{
	void TownController::Setup(UIManager* ui, DungeonGameState* dungeonGameState, StateMachine<EGameState>* globalSM)
	{
		m_UI = ui;
		m_DungeonGameState = dungeonGameState;
		m_GlobalStateMachine = globalSM;

		m_UI->OnTownActionSelected.AddListener([this](int choice) {
			_handleTownAction(choice);
		});
	}

	void TownController::Process()
	{
		m_UI->PromptTownAction();
	}

	void TownController::_handleTownAction(int choice)
	{
		switch (choice) {
		case 1: // 전직소
			m_GlobalStateMachine->PushState(EGameState::GS_JOB_CENTER);
			break;
		case 2: // 포션 상점
			m_GlobalStateMachine->PushState(EGameState::GS_SHOP);
			break;
		case 0: // 마을 나가기
			m_GlobalStateMachine->PopState();
			m_UI->PrintMessage("Leaving town.");
			break;
		default: m_UI->PrintMessage("Invalid choice. Please try again."); break;
		}
	}
}