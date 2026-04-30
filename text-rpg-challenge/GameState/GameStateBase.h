#pragma once

#include "../Data/EGameState.h"
#include "../Unit/Player/Player.h"



namespace TextRPG
{
	namespace GameState
	{
		class GameStateBase
		{
		protected:
			Data::EGameState CurrentState = Data::EGameState::GS_NONE;

		public:
			virtual void SaveData() = 0;
			virtual void LoadData() = 0;
		};
	}
}
