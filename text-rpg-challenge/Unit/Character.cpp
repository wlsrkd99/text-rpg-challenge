#include "Character.h"



namespace TextRPG
{
	namespace Unit
	{
		const std::string& Character::GetName() const
		{
			return m_Name;
		}
		
		int Character::GetMaxHP() const { return m_MaxHp; }
		int Character::GetMaxMP() const { return m_MaxMp; }
		int Character::GetHP() const { return m_Hp; }
		int Character::GetMP() const { return m_Mp; }
		int Character::GetAttack() const { return m_Atk; }
		int Character::GetDefense() const { return m_Def; }
	}
}
