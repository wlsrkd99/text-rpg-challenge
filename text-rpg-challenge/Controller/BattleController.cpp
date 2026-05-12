#include "BattleController.h"
#include "../Unit/Player/Player.h"
#include "../Unit/Monster/Monster.h"
#include "../Data/BattleData.h"
#include "../Item/ItemBase.h"

namespace TextRPG
{
	void BattleController::Setup(UIManager* ui, DungeonGameState* dungeonGameState, StateMachine<EGameState>* globalSM)
	{
		m_UI = ui;
		m_DungeonGameState = dungeonGameState;
		m_GlobalStateMachine = globalSM;

		m_UI->OnBattleActionSelected.AddListener([this](EBattleActionType actionType) {
			_handleBattleAction(actionType);
		});

		m_UI->OnItemUseRequested.AddListener([this](int itemID) {
			_handleItemUseResult(itemID);
		});
	}

	void BattleController::Process()
	{
		Room* currentRoom = m_DungeonGameState->GetCurrentRoom();
		if (!currentRoom || !currentRoom->RoomMonster)
		{
			m_GlobalStateMachine->PopState();
			return;
		}

		Monster* monster = currentRoom->RoomMonster;
		Player* player = m_DungeonGameState->GetUser().GetPlayer();

		m_UI->PrintTitle("[ Battle Start! ] " + player->GetName() + " vs " + monster->GetName());

		bool isBattleOver = false;
		bool isPlayerTurn = true;

		while (!isBattleOver)
		{
			if (isPlayerTurn)
			{
				m_bPlayerTurnConsumed = false;
				m_UI->PromptBattleAction();
				if (!m_bPlayerTurnConsumed) continue;

				if (monster->GetCurrentHP() <= 0)
				{
					m_UI->PrintMessage(monster->GetName() + " has been defeated!");
					currentRoom->bIsCleared = true;

					BattleWinData winData;
					winData.EarnedExp = monster->GetDropExp();
					winData.EarnedGold = monster->GetDropGold();
					if (monster->GetDropItem() != nullptr)
					{
						winData.Rewards.push_back(monster->ReleaseDropItem());
					}

					m_UI->DisplayBattleWinDetails(winData);
					m_DungeonGameState->GetUser().GetInventory().GainGold(winData.EarnedGold);
					if (winData.Rewards.size() > 0)
					{
						for(ItemBase* item : winData.Rewards)
						{
							m_DungeonGameState->GetUser().GetInventory().AddItem(item);
						}
					}
					player->AddExp(winData.EarnedExp);
					isBattleOver = true;
				}
			}
			else
			{
				m_UI->PrintMessage("");
				m_UI->PrintMessage("--- Monster Turn ---");
				m_UI->DisplayAttackResult(monster->Attack(*player));

				if(player->GetCurrentHP() <= 0)
				{
					m_UI->PrintMessage(player->GetName() + " has been defeated!");
					BattleLoseData loseData;
					m_UI->DisplayBattleLoseDetails(loseData);
					isBattleOver = true;
					m_GlobalStateMachine->ChangeState(EGameState::GS_GAMEOVER);
				}
			}
			isPlayerTurn = !isPlayerTurn;
		}
		m_GlobalStateMachine->PopState();
	}

	void BattleController::_handleBattleAction(EBattleActionType actionType)
	{
		Player* player = m_DungeonGameState->GetUser().GetPlayer();
		Monster* monster = m_DungeonGameState->GetCurrentRoom()->RoomMonster;

		switch (actionType)
		{
		case EBattleActionType::BAT_ATTACK:
			m_UI->DisplayAttackResult(player->Attack(*monster));
			m_bPlayerTurnConsumed = true;
			break;
		case EBattleActionType::BAT_SKILL:
			m_UI->PrintMessage("TODO Skill (implemented)");
			break;
		case EBattleActionType::BAT_ITEM:
			m_UI->DisplayInventory(m_DungeonGameState->GetUser().GetInventory());
			m_bItemUsedSuccessfully = false;
			m_GlobalStateMachine->PushState(EGameState::GS_INVENTORY_USE);
			break;
		case EBattleActionType::BAT_RUN:
			m_UI->PrintMessage("You attempt to run away!");
			// TODO: 도주 로직 구현 (성공 시 PopState, 실패 시 턴 소모)
			m_bPlayerTurnConsumed = true;
			break;
		default:
			m_UI->PrintMessage("Invalid choice. Please try again.");
			break;
		}
	}

	void BattleController::_handleItemUseResult(int itemID)
	{
		if (m_GlobalStateMachine->GetCurrentState() == EGameState::GS_INVENTORY_USE)
		{
			if (itemID == -1) // 아이템 사용 취소
			{
				m_bItemUsedSuccessfully = false;
			}
			else // 아이템 사용 시도
			{
				Player* player = m_DungeonGameState->GetUser().GetPlayer();
				ItemUseResult useResult = m_DungeonGameState->GetUser().UseItem(itemID, *player);
				m_UI->DisplayItemUseResult(useResult);
				m_bItemUsedSuccessfully = useResult.bSuccess;
			}
			m_GlobalStateMachine->PopState();
			if (m_bItemUsedSuccessfully)
			{
				m_bPlayerTurnConsumed = true;
			}
		}
	}
}
