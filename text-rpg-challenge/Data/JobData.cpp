#include "JobData.h"
#include "../Enums/EJobType.h"
#include "../Enums/EStatType.h"

namespace TextRPG
{
    const std::map<EJobType, std::vector<BaseStatBonus>> JobBonuses = {
        {EJobType::JT_WARRIOR, {
            {EStatType::ST_Strength, 5},
            {EStatType::ST_Endurance, 5}
        }},
        {EJobType::JT_MAGE, {
            {EStatType::ST_Intelligence, 5},
            {EStatType::ST_Wisdom, 5}
        }},
        {EJobType::JT_ROGUE, {
            {EStatType::ST_Agility, 7},
            {EStatType::ST_Strength, 3}
        }},
        {EJobType::JT_ARCHER, {
            {EStatType::ST_Agility, 5},
            {EStatType::ST_Wisdom, 5}
        }}
    };

}