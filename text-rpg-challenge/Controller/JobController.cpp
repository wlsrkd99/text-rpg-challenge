#include "JobController.h"
#include "../Unit/Player/Player.h"

namespace TextRPG
{
	void JobController::Setup(UIManager* ui, DungeonGameState* dungeonGameState, StateMachine<EGameState>* globalSM)
	{
		m_UI = ui;
		m_DungeonGameState = dungeonGameState;
		m_GlobalStateMachine = globalSM;

		m_UI->OnJobChangeRequested.AddListener([this](int choice) {
			_handleJobChangeRequest(choice);
		});
	}

	void JobController::Process()
	{
		Player* player = m_DungeonGameState->GetUser().GetPlayer();
        if (player->GetJob() != EJobType::JT_NOVICE)
		{
			m_UI->PrintMessage("You already have a job. Cannot change job again.");
			return;
		}
		m_UI->PromptJobChange(*player);
	}

	void JobController::_handleJobChangeRequest(int choice)
	{
		Player* player = m_DungeonGameState->GetUser().GetPlayer();
		
		if (choice == 0)
		{
			m_UI->PrintMessage("You already have a job.");
			m_GlobalStateMachine->PopState();
			return;
		}

		if (choice < static_cast<int>(EJobType::JT_WARRIOR) || choice >= static_cast<int>(EJobType::JT_MAX))
		{
			m_UI->PrintMessage("Invalid choice. You remain a Novice for now.");
			return;
		}

		EJobType selectedJob = static_cast<EJobType>(choice);
		std::string bonusMessage = GetJobName(selectedJob);
		player->ChangeJob(selectedJob);

		m_UI->PrintMessage("* You became a " + GetJobName(selectedJob) + "! (" + bonusMessage + ")");
		m_UI->DisplayCharacterSheet(*player);
		m_GlobalStateMachine->PopState();
	}
}
