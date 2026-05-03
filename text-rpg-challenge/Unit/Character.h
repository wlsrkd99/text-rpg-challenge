#pragma once

#include <string>
#include "../Data/EStatType.h"
#include <array>

namespace TextRPG
{
	class Character
	{
	protected:
		std::string m_Name;

		// 기본 스탯 (유저가 투자하는 스탯)
		std::array<int, static_cast<int>(EStatType::ST_Count)> m_BaseStats;

		// 파생 스탯 (기본 스탯에 의해 계산되는 스탯)
		int m_CurrentHP;
		int m_CurrentMP;
		int m_MaxHP;
		int m_MaxMP;
		int m_PAtk; // 물리 공격력
		int m_PDef; // 물리 방어력
		int m_MAtk; // 마법 공격력
		int m_MDef; // 마법 방어력
		int m_Speed; // 속도

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

		const std::string& GetName() const { return m_Name; }
		const std::array<int, static_cast<int>(EStatType::ST_Count)>& GetBaseStats() const { return m_BaseStats; }
		/** @brief 특정 기본 스탯 값을 가져옵니다. */
		int GetBaseStat(EStatType stat) const;
		int GetCurrentHP() const { return m_CurrentHP; }
		int GetCurrentMP() const { return m_CurrentMP; }
		int GetMaxHP() const;
		int GetMaxMP() const;
		int GetPAtk() const { return m_PAtk; }
		int GetPDef() const { return m_PDef; }
		int GetMAtk() const { return m_MAtk; }
		int GetMDef() const { return m_MDef; }
		int GetSpeed() const { return m_Speed; }

		void SetName(const std::string& name) { m_Name = name; }
		/** @brief 특정 기본 스탯을 설정하고 파생 스탯을 업데이트합니다. */
		void SetBaseStat(EStatType stat, int value);

		virtual void TakeDamage(int damage);
		virtual void Heal(int amount);
		virtual void UseMP(int amount);
	};
}
