#pragma once

#include "../Unit/Player/Player.h"
#include "../Enums/EGameState.h"
#include "../Core/User.h"

namespace TextRPG
{
	class GameStateBase
	{
	protected:
		User m_User;
		EGameState CurrentState = EGameState::GS_NONE;

	public:
		GameStateBase() : m_User("Hero") { CurrentState = EGameState::GS_NONE; }

		EGameState GetCurrentState() const { return CurrentState; }
		void SetCurrentState(EGameState state) { CurrentState = state; }

		User& GetUser() { return m_User; }
		Player* GetPlayer() { return m_User.GetPlayer(); }
	};
}
