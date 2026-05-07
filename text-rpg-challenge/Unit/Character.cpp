#include "Character.h"
#include "../Data/StatData.h"
#include "../Data/BattleData.h"
#include <algorithm>
#include <vector>

namespace TextRPG
{
	void Character::UpdateDerivedStats()
	{
		// 파생 스탯을 기본값으로 초기화
		m_MaxHP = 0;
		m_MaxMP = 0;
		m_PAtk = 10;
		m_PDef = 10;
		m_MAtk = 10;
		m_MDef = 10;
		m_Speed = 0;

		// StatData에 정의된 규칙에 따라 파생 스탯 계산
		for (int i = 0; i < static_cast<int>(EStatType::ST_Count); ++i)
		{
			EStatType baseStatType = static_cast<EStatType>(i);
			int baseStatValue = m_BaseStats[i];

			auto it = BaseStatEffects.find(baseStatType);
			if (it != BaseStatEffects.end())
			{
				for (const auto& effect : it->second)
				{
					switch (effect.DerivedStat)
					{
					case EDerivedStatType::MaxHP: m_MaxHP += baseStatValue * effect.Multiplier; break;
					case EDerivedStatType::MaxMP: m_MaxMP += baseStatValue * effect.Multiplier; break;
					case EDerivedStatType::PAtk:  m_PAtk += baseStatValue * effect.Multiplier; break;
					case EDerivedStatType::PDef:  m_PDef += baseStatValue * effect.Multiplier; break;
					case EDerivedStatType::MAtk:  m_MAtk += baseStatValue * effect.Multiplier; break;
					case EDerivedStatType::MDef:  m_MDef += baseStatValue * effect.Multiplier; break;
					case EDerivedStatType::Speed: m_Speed += baseStatValue * effect.Multiplier; break;
					}
				}
			}
		}

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

	AttackResult Character::Attack(Character& target)
	{
		AttackResult result;
		result.AttackerName = GetName();
		result.AttackName = "attacks";
		result.TargetName = target.GetName();
		result.TargetHP = target.GetCurrentHP();

		int damage = std::max(1, GetPAtk() - target.GetPDef());
		target.TakeDamage(damage);
		std::vector<int> damages { damage };
		result.Damages = damages;

		return result;
	}

	bool Character::TakeDamage(int damage)
	{
		if (damage < 0) damage = 0;
		m_CurrentHP -= damage;
		return m_CurrentHP <= 0;
	}

	int Character::RestoreHealth(int amount)
	{
		if (amount < 0) amount = 0;
		int oldHP = m_CurrentHP;
		m_CurrentHP += amount;
		if (m_CurrentHP > m_MaxHP) m_CurrentHP = m_MaxHP;
		return m_CurrentHP - oldHP;
	}

	bool Character::UseMP(int amount)
	{
		if (amount < 0) amount = 0;
		if (m_CurrentMP < amount)
		{
			return false;
		}
		m_CurrentMP -= amount;
		if (m_CurrentMP < 0) m_CurrentMP = 0;
		return true;
	}

	int Character::RestoreMana(int amount)
	{
		if (amount < 0) amount = 0;
		int oldMP = m_CurrentMP;
		m_CurrentMP += amount;
		if (m_CurrentMP > m_MaxMP) m_CurrentMP = m_MaxMP;
		return m_CurrentMP - oldMP;
	}
}
