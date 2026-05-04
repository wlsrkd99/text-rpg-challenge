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
			return; // Novice에서만 다른 직업으로 전직할 수 있습니다.
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

		m_CurrentHP = GetMaxHP();
		m_CurrentMP = GetMaxMP();
	}
}
