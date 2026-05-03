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
		DungeonGameState() : m_User("DefaultPlayer", FStatContainer(100, 50, 10, 5)) {}
		DungeonGameState(const std::string& playerName, const FStatContainer& playerStats)
			: m_User(playerName, playerStats) { }

		virtual void SaveData();
		virtual void LoadData();

		/** @brief 게임 상태 내의 메인 플레이어 객체 포인터를 반환합니다. */
		Player* GetPlayer() { return m_User.GetPlayer(); }
	};
}
