#include "Player.h"

namespace TextRPG
{
	bool Player::SpendStatPoints(int points)
	{
		if (points <= m_AvailableStatPoints) 
		{
			m_AvailableStatPoints -= points; 
			return true; 
		}
		return false;
	}
}
