#pragma once

#include <string>
#include "../Data/EStatType.h"

namespace TextRPG
{
	struct FStatContainer
	{
		int MaxHp;
		int MaxMp;
		int Atk;
		int Def;

		FStatContainer(int InMaxHp = 0, int InMaxMp = 0, int InAtk = 0, int InDef = 0)
			: MaxHp(InMaxHp), MaxMp(InMaxMp), Atk(InAtk), Def(InDef) { }
	};


	class Character
	{
	protected:
		std::string m_Name;
		FStatContainer m_BaseStats;
		int m_Stats[static_cast<int>(EStatType::Count)];

	public:
		Character(const std::string& name, const FStatContainer& stats)
			: m_Name(name), m_BaseStats(stats)
		{
			m_Stats[static_cast<int>(EStatType::Hp)] = stats.MaxHp;
			m_Stats[static_cast<int>(EStatType::Mp)] = stats.MaxMp;
			m_Stats[static_cast<int>(EStatType::Atk)] = stats.Atk;
			m_Stats[static_cast<int>(EStatType::Def)] = stats.Def;
		}

		void Initalize(const FStatContainer& stats)
		{
			m_BaseStats = stats;
			m_Stats[static_cast<int>(EStatType::Hp)] = stats.MaxHp;
			m_Stats[static_cast<int>(EStatType::Mp)] = stats.MaxMp;
			m_Stats[static_cast<int>(EStatType::Atk)] = stats.Atk;
			m_Stats[static_cast<int>(EStatType::Def)] = stats.Def;
		}

		const std::string& GetName() const { return m_Name; }
		void SetName(const std::string& name) { m_Name = name; }
		int GetMaxHP() const;
		int GetMaxMP() const;
		int GetStat(EStatType stat) const;

		virtual void TakeDamage(float damage) = 0;
		virtual void Attack(float damage) = 0;
	};
}
