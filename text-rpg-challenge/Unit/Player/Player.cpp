#include "Player.h"
#include "../../Enums/EJobType.h"
#include "../../Data/JobData.h"

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

	void Player::LevelUp()
	{
		// TODO: 레벨업 로직 구현 (스탯 포인트 지급 등)
	}

	void Player::GainExp(int exp)
	{
		m_Exp += exp;
		// TODO: 경험치에 따른 레벨업 체크 로직 구현
	}

	void Player::ChangeJob(EJobType newJob)
	{
		if (m_Job != EJobType::JT_NOVICE || newJob == EJobType::JT_NOVICE)
		{
			return;
		}

		m_Job = newJob;

		auto it = JobBonuses.find(newJob);
		if (it != JobBonuses.end())
		{
			for (const auto& bonus : it->second)
			{
				SetBaseStat(bonus.StatType, GetBaseStat(bonus.StatType) + bonus.Amount);
			}
		}

		switch (newJob)
		{
			case EJobType::JT_WARRIOR: m_AttackStrategy = std::make_unique<WarriorAttackStrategy>(); break;
			case EJobType::JT_ROGUE:   m_AttackStrategy = std::make_unique<RogueAttackStrategy>(); break;
			case EJobType::JT_MAGE:    m_AttackStrategy = std::make_unique<MageAttackStrategy>(); break;
			case EJobType::JT_ARCHER:  m_AttackStrategy = std::make_unique<ArcherAttackStrategy>(); break;
			default: m_AttackStrategy = std::make_unique<NoviceAttackStrategy>(); break;
		}

		m_CurrentHP = GetMaxHP();
		m_CurrentMP = GetMaxMP();
	}

	void Player::Attack(Character& target)
	{
		if (m_AttackStrategy)
		{
			m_AttackStrategy->ExecuteAttack(this, target);
		}
		else
		{
			Character::Attack(target);
		}
	}
}
