#pragma once

#include "../Character.h"

namespace TextRPG
{
	class Player : public Character
	{
	private:
		int m_Level = 1;
		long m_Exp = 0;
	public:
		Player() : Character("Player") {}
	};
}
