#include "Character.h"



namespace TextRPG
{
	int Character::GetMaxHP() const { return m_BaseStats.MaxHp; }
	int Character::GetMaxMP() const { return m_BaseStats.MaxMp; }
	int Character::GetStat(EStatType stat) const
	{
		if (stat == EStatType::Hp || stat == EStatType::Mp || stat == EStatType::Atk || stat == EStatType::Def)
		{
			return m_Stats[static_cast<int>(stat)];
		}
		return 0;
	}
}
