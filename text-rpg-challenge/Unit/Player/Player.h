#pragma once

#include "../Character.h"

namespace TextRPG
{
	class Player : public Character
	{
	private:
		int m_Level = 1;
		long m_Exp = 0;

		int m_HPPotionCount = 5; // 초기 포션 개수
		int m_MPPotionCount = 5; // 초기 포션 개수

	public:
		Player() : Character("Player") {}
	};
}
