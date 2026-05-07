#pragma once

#include "../Enums/EStatType.h"
#include <string>
#include <array>

namespace TextRPG
{
	// Forward declaration for AttackResult
	struct AttackResult;

	class Character
	{
	protected:
		std::string m_Name;

		// 기본 스탯
		std::array<int, static_cast<int>(EStatType::ST_Count)> m_BaseStats;

		// 파생 스탯
		int m_CurrentHP;
		int m_CurrentMP;
		int m_MaxHP;
		int m_MaxMP;
		int m_PAtk; 
		int m_PDef; 
		int m_MAtk; 
		int m_MDef; 
		int m_Speed;

	private:
		/** @brief 기본 스탯을 기반으로 파생 스탯을 다시 계산합니다. */
		void UpdateDerivedStats();

	public:
		Character(const std::string& name = "Character")
			: m_Name(name)
		{
			m_BaseStats.fill(0);
			UpdateDerivedStats();
		}

		/** @brief 이름과 기본 스탯으로 캐릭터를 초기화합니다. */
		void Initialize(const std::string& name, const std::array<int, static_cast<int>(EStatType::ST_Count)>& stats);

		int GetCurrentHP() const { return m_CurrentHP; }
		int GetCurrentMP() const { return m_CurrentMP; }
		int GetMaxHP() const;
		int GetMaxMP() const;
		int GetPAtk() const { return m_PAtk; }
		int GetPDef() const { return m_PDef; }
		int GetMAtk() const { return m_MAtk; }
		int GetMDef() const { return m_MDef; }
		int GetSpeed() const { return m_Speed; }

		const std::string& GetName() const { return m_Name; }
		void SetName(const std::string& name) { m_Name = name; }
		/** @brief 특정 기본 스탯 값을 가져옵니다. */
		int GetBaseStat(EStatType stat) const;
		/** @brief 특정 기본 스탯을 설정하고 파생 스탯을 업데이트합니다. */
		void SetBaseStat(EStatType stat, int value);
		const std::array<int, static_cast<int>(EStatType::ST_Count)>& GetBaseStats() const { return m_BaseStats; }

		virtual AttackResult Attack(Character& target);
		virtual bool TakeDamage(int damage);
		virtual int RestoreHealth(int amount);
		virtual bool UseMP(int amount);
		virtual int RestoreMana(int amount);
	};
}
