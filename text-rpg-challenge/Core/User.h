#pragma once
#include "../Unit/Player/Player.h"


namespace TextRPG
{
	class User
	{
	private:
		Player m_MainPlayer;

	public:
		User(const std::string& name) : m_MainPlayer()
		{
			m_MainPlayer.SetName(name);
		}

		/** @brief 게임 상태 내의 메인 플레이어 객체 포인터를 반환합니다. */
		Player* GetPlayer() { return &m_MainPlayer; }
	};
}
