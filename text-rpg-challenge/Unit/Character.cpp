#include "Character.h"
#include <algorithm>

namespace TextRPG
{
	void Character::UpdateDerivedStats()
	{
		// 파생 스탯 계산 공식
		m_MaxHP = (m_BaseStats[static_cast<int>(EStatType::ST_Health)] * 5) + (m_BaseStats[static_cast<int>(EStatType::ST_Endurance)] * 2) + (m_BaseStats[static_cast<int>(EStatType::ST_Strength)]);
		m_MaxMP = (m_BaseStats[static_cast<int>(EStatType::ST_Mana)] * 5) + (m_BaseStats[static_cast<int>(EStatType::ST_Intelligence)] * 2) + (m_BaseStats[static_cast<int>(EStatType::ST_Wisdom)]);
		m_Speed = m_BaseStats[static_cast<int>(EStatType::ST_Agility)];
		m_PAtk = 10 + m_BaseStats[static_cast<int>(EStatType::ST_Strength)] * 2;
		m_PDef = 10 + m_BaseStats[static_cast<int>(EStatType::ST_Endurance)] * 2;
		m_MAtk = 10 + m_BaseStats[static_cast<int>(EStatType::ST_Intelligence)] * 2;
		m_MDef = 10 + m_BaseStats[static_cast<int>(EStatType::ST_Wisdom)] * 2;

		m_CurrentHP = std::min(m_CurrentHP, m_MaxHP);
		m_CurrentMP = std::min(m_CurrentMP, m_MaxMP);
	}

	void Character::Initialize(const std::string& name, const std::array<int, static_cast<int>(EStatType::ST_Count)>& stats)
	{
		m_Name = name;
		m_BaseStats = stats;
		UpdateDerivedStats();

		m_CurrentHP = m_MaxHP;
		m_CurrentMP = m_MaxMP;
	}

	int Character::GetMaxHP() const { return m_MaxHP; }
	int Character::GetMaxMP() const { return m_MaxMP; }

	int Character::GetBaseStat(EStatType stat) const
	{
		if (stat < EStatType::ST_Count)
			return m_BaseStats[static_cast<int>(stat)];

		// TODO : 예외 처리 - 유효하지 않은 stat 요청
		return 0; 
	}

	void Character::SetBaseStat(EStatType stat, int value)
	{
		if (stat < EStatType::ST_Count && value >= 0)
		{
			m_BaseStats[static_cast<int>(stat)] = value;
			UpdateDerivedStats();
		}
	}

	void Character::TakeDamage(int damage)
	{
		m_CurrentHP -= damage;
		if (m_CurrentHP < 0) m_CurrentHP = 0;
	}

	void Character::Heal(int amount)
	{
		m_CurrentHP += amount;
		if (m_CurrentHP > m_MaxHP) m_CurrentHP = m_MaxHP;
	}

	void Character::UseMP(int amount)
	{
		m_CurrentMP -= amount;
		if (m_CurrentMP < 0) m_CurrentMP = 0;
	}
}
