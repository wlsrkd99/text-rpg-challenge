#pragma once

#include "../Character.h"
#include "../../Enums/EJobType.h"
#include "AttackStrategy.h"
#include <memory>

namespace TextRPG
{
	class Player : public Character
	{
	protected:
		EJobType m_Job;

	private:
		int m_Level;
		long m_Exp;
		int m_AvailableStatPoints;
		std::unique_ptr<IAttackStrategy> m_AttackStrategy;

	public:
		Player() : Character("Player") 
		{
			m_Level = 1;
			m_Exp = 0;
			m_AvailableStatPoints = 20;
			m_Job = EJobType::JT_NOVICE;
			m_AttackStrategy = std::make_unique<NoviceAttackStrategy>();
		}
		int GetAvailableStatPoints() const { return m_AvailableStatPoints; }
		int GetLevel() const { return m_Level; }
		virtual EJobType GetJob() const { return m_Job; }
		bool SpendStatPoints(int points);

		bool LevelUp();
		int GetExpToNextLevel() const;
		long GetExp() const { return m_Exp; }
		void AddExp(int exp);
		void SetJob(EJobType job) { m_Job = job; }
		void ChangeJob(EJobType newJob);
		
		AttackResult Attack(Character& target) override;
	};
}
