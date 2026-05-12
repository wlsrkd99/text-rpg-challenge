#include "InventoryController.h"
#include "../Unit/Player/Player.h"
#include "../Data/BattleData.h"

namespace TextRPG
{
	void InventoryController::Setup(UIManager* ui, DungeonGameState* dungeonGameState, StateMachine<EGameState>* globalSM)
	{
		m_UI = ui;
		m_DungeonGameState = dungeonGameState;
		m_GlobalStateMachine = globalSM;

		m_UI->OnInventoryActionSelected.AddListener([this](int choice) {
			_handleInventoryAction(choice);
		});

		m_UI->OnItemUseRequested.AddListener([this](int itemID) {
			if (m_GlobalStateMachine->GetCurrentState() == EGameState::GS_INVENTORY_USE)
			{
				_handleItemUse(itemID);
			}
		});
	}

	void InventoryController::Process()
	{
		Inventory& playerInventory = m_DungeonGameState->GetUser().GetInventory();

		switch (m_GlobalStateMachine->GetCurrentState())
		{
		case EGameState::GS_INVENTORY:
			m_UI->PromptInventoryAction(playerInventory);
			break;
		case EGameState::GS_INVENTORY_USE:
			m_UI->DisplayInventory(playerInventory);
			m_UI->PromptItemUse(playerInventory);
			break;
		default:
			// 이 컨트롤러가 처리할 상태가 아님.
			// 이 경우는 발생하지 않아야 하지만, 방어적 코딩.
			m_GlobalStateMachine->PopState();
			break;
		}
	}

	void InventoryController::_handleInventoryAction(int choice)
	{
		switch (choice) {
		case 1: m_GlobalStateMachine->PushState(EGameState::GS_INVENTORY_USE); break;
		case 0: m_GlobalStateMachine->PopState(); break;
		default: m_UI->PrintMessage("Invalid choice. Please try again."); break;
		}
	}

	void InventoryController::_handleItemUse(int itemID)
	{
		if (itemID == -1) { // 아이템 사용 취소
			m_GlobalStateMachine->PopState();
			return;
		}
		Player* player = m_DungeonGameState->GetUser().GetPlayer();
		ItemUseResult useResult = m_DungeonGameState->GetUser().UseItem(itemID, *player);
		m_UI->DisplayItemUseResult(useResult);
		m_GlobalStateMachine->PopState();
	}
}
