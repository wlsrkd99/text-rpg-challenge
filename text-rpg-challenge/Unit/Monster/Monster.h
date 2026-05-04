#pragma once

#include"../Character.h"


namespace TextRPG
{
	class Monster : public Character
	{
	private:
		long m_DropExp;
		
	public:
		Monster(const std::string& name = "Monster") : Character(name)
		{
			m_DropExp = 0;
		}
		long GetDropExp() const { return m_DropExp; }
		void SetDropExp(long exp) { m_DropExp = exp; }
	};
}
