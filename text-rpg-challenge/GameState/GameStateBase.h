#pragma once

#include "../Unit/Player/Player.h"
#include "../Enums/EGameState.h"



namespace TextRPG
{
	class GameStateBase
	{
	protected:
		EGameState CurrentState = EGameState::GS_NONE;

	public:
		GameStateBase() { }
		virtual void SaveData() = 0;
		virtual void LoadData() = 0;
	};
}
