#include "DungeonController.h"
#include "../Data/DungeonData.h"

namespace TextRPG
{
	void DungeonController::Setup(UIManager* ui, DungeonGameState* dungeonGameState, StateMachine<EGameState>* globalSM)
	{
		m_UI = ui;
		m_DungeonGameState = dungeonGameState;
		m_GlobalStateMachine = globalSM;

		m_UI->OnDungeonRoomSelected.AddListener([this](int choice, int maxIndex) {
			_handleDungeonRoomSelection(choice, maxIndex);
		});
	}

	void DungeonController::Process()
	{
		if (m_DungeonGameState && m_DungeonGameState->GetCurrentFloor())
		{
			m_UI->PromptDungeonAction(*m_DungeonGameState->GetCurrentFloor());
		}
	}

	void DungeonController::_handleDungeonRoomSelection(int choice, int maxIndex)
	{
		if (choice == 0)
		{
			m_GlobalStateMachine->ChangeState(EGameState::GS_MAIN_MENU);
		}
		else if (choice > 0 && choice <= maxIndex)
		{
			Room* selectedRoom = m_DungeonGameState->GetCurrentFloor()->GetRoom(choice);
			if (selectedRoom->bIsCleared)
			{
				m_UI->PrintMessage("This room has already been cleared.");
				return;
			}
			m_DungeonGameState->SetCurrentRoom(selectedRoom);
			m_GlobalStateMachine->PushState(EGameState::GS_BATTLE);
		}
		else
		{
			m_UI->PrintMessage("Invalid choice. Please try again.");
		}
	}
}