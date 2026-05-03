#pragma once

#include "GameStateBase.h"
#include "../Unit/Player/Player.h"
#include "../Core/User.h"

namespace TextRPG
{
	class DungeonGameState : public GameStateBase
	{
	private:
		User m_User;

	public:
		DungeonGameState() : m_User("Hero") { CurrentState = EGameState::GS_NONE; }

		virtual void SaveData();
		virtual void LoadData();

		User& GetUser() { return m_User; }
		Player* GetPlayer() { return m_User.GetPlayer(); }
	};
}
