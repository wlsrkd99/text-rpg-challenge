#include "StatController.h"
#include "../Unit/Player/Player.h"
#include "../Data/StatData.h"
#include "../Utils/StatFormatting.h"

namespace TextRPG
{
	void StatController::Setup(UIManager* ui, DungeonGameState* dungeonGameState, StateMachine<EGameState>* globalSM)
	{
		m_UI = ui;
		m_DungeonGameState = dungeonGameState;
		m_GlobalStateMachine = globalSM;

		m_UI->OnStatUpgradeMenuSelected.AddListener([this](int choice) {
			_handleStatUpgradeMenuSelection(choice);
		});

		m_UI->OnStatUpgradeRequested.AddListener([this](EStatType stat, int amount) {
			_handleStatUpgradeRequest(stat, amount);
		});
	}

	void StatController::Process()
	{
		Player* player = m_DungeonGameState->GetUser().GetPlayer();

		switch (m_GlobalStateMachine->GetCurrentState())
		{
		case EGameState::GS_STAT_UPGRADE:
			m_UI->PromptStatUpgradeAction(*player);
			break;
		case EGameState::GS_STAT_ALLOCATION:
		{
			std::vector<std::string> coreStatNames;
			for (EStatType statType : UpgradeableCoreStats) coreStatNames.push_back(GetStatName(statType));
			m_UI->PromptStatAllocation(coreStatNames, player->GetBaseStats(), player->GetAvailableStatPoints());
			break;
		}
		default:
			// 이 컨트롤러가 처리할 상태가 아님.
			// 이 경우는 발생하지 않아야 하지만, 방어적 코딩.
			m_GlobalStateMachine->PopState();
			break;
		}
	}

	void StatController::_handleStatUpgradeMenuSelection(int choice)
	{
		Player* player = m_DungeonGameState->GetUser().GetPlayer();
		switch (choice) {
		case 1: m_UI->DisplayStatEffects(); break;
		case 2: 
			if (player->GetAvailableStatPoints() <= 0) m_UI->PrintMessage("You have no available stat points to spend.");
			else m_GlobalStateMachine->PushState(EGameState::GS_STAT_ALLOCATION);
			break;
		case 0: m_GlobalStateMachine->PopState(); m_UI->PrintMessage("Returning."); break;
		default: m_UI->PrintMessage("Invalid choice. Please try again."); break;
		}
	}

	void StatController::_handleStatUpgradeRequest(EStatType stat, int amount)
	{
		Player* player = m_DungeonGameState->GetUser().GetPlayer();

		if (stat == EStatType::ST_Count && amount == 0) { // 스탯 분배 취소 (0 입력)
			m_GlobalStateMachine->PopState();
			return;
		}

		if (player->SpendStatPoints(amount)) {
			player->SetBaseStat(stat, player->GetBaseStat(stat) + amount);
			m_UI->PrintMessage(GetStatName(stat) + " increased by " + std::to_string(amount) + ".");
			m_UI->DisplayCharacterSheet(*player);
			
			if (player->GetAvailableStatPoints() <= 0) // 남은 포인트가 없으면 스탯 분배 종료
				m_GlobalStateMachine->PopState();
		}
		else {
			m_UI->PrintMessage("You don't have enough points. You only have " + std::to_string(player->GetAvailableStatPoints()) + " left.");
		}
	}
}
