#pragma once

#include "../Character.h"

namespace TextRPG
{
	class Player : public Character
	{
	private:
		int m_Level = 1;
		long m_Exp = 0;
		int m_AvailableStatPoints = 20;
	public:
		Player() : Character("Player") {}
		int GetAvailableStatPoints() const { return m_AvailableStatPoints; }

		//void AddStatPoints(int points) { m_AvailableStatPoints += points; }
		bool SpendStatPoints(int points);
	};
}
