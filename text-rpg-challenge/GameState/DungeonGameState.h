#pragma once

#include "GameStateBase.h"

namespace TextRPG
{
	class DungeonGameState : public GameStateBase
	{
	public:
		DungeonGameState() { CurrentState = EGameState::GS_NONE; }
	};
}
