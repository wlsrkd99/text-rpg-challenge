#include "StatData.h"

namespace TextRPG
{
    const std::map<EStatType, std::vector<StatEffect>> BaseStatEffects = {
        { EStatType::ST_Health,      { { EDerivedStatType::MaxHP, 5 } } },
        { EStatType::ST_Mana,        { { EDerivedStatType::MaxMP, 5 } } },
        { EStatType::ST_Endurance,   { { EDerivedStatType::MaxHP, 2 }, { EDerivedStatType::PDef, 2 } } },
        { EStatType::ST_Strength,    { { EDerivedStatType::MaxHP, 1 }, { EDerivedStatType::PAtk, 2 } } },
        { EStatType::ST_Agility,     { { EDerivedStatType::Speed, 1 } } },
        { EStatType::ST_Intelligence,{ { EDerivedStatType::MaxMP, 2 }, { EDerivedStatType::MAtk, 2 } } },
        { EStatType::ST_Wisdom,      { { EDerivedStatType::MaxMP, 1 }, { EDerivedStatType::MDef, 2 } } }
    };

    const std::vector<EStatType> UpgradeableCoreStats = {
        EStatType::ST_Endurance,
        EStatType::ST_Strength,
        EStatType::ST_Agility,
        EStatType::ST_Intelligence,
        EStatType::ST_Wisdom
    };
}