#pragma once

#include "GameStateBase.h"
#include "../Data/DungeonData.h"

namespace TextRPG
{
	class DungeonGameState : public GameStateBase
	{
	private:
		DungeonFloor m_CurrentFloor;
		Room* m_CurrentRoom = nullptr;

	public:
		DungeonGameState() { CurrentState = EGameState::GS_NONE; }

		DungeonFloor* GetCurrentFloor() { return &m_CurrentFloor; }
		Room* GetCurrentRoom() const { return m_CurrentRoom; }
		void SetCurrentRoom(Room* room) { m_CurrentRoom = room; }
	};
}
