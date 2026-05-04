#pragma once
#include "../Enums/EStatType.h"


namespace TextRPG
{
    struct BaseStatBonus
    {
        EStatType StatType;
        int Amount;
    };
}