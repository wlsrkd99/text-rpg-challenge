#include "Player.h"

namespace TextRPG
{
	namespace Unit
	{
		void Player::SetInfo(const std::string& name, int hp, int mp, int atk, int def)
		{
			TextRPG::Unit::Character::m_Name = name;
			m_MaxHp = hp;
			m_Hp = hp;
			m_MaxMp = mp;
			m_Mp = mp;
			m_Atk = atk;
			m_Def = def;
		}

		void Player::TakeDamage(float damage)
		{
			// TODO: 데미지 계산 로직 구현
		}

		void Player::Attack(float damage)
		{
			// TODO: 공격 로직 구현
		}
}
}
