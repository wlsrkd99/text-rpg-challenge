#pragma once

#include "../Enums/EStatType.h"
#include "../Enums/EDerivedStatType.h"
#include <map>
#include <vector>

namespace TextRPG
{
    struct StatEffect
    {
        EDerivedStatType DerivedStat;
        int Multiplier;
    };

    /** @brief 기본스탯별 파생스탯 계산용 목록 */
    extern const std::map<EStatType, std::vector<StatEffect>> BaseStatEffects;

    /** @brief 투자가능한 기본스탯 목록 */
    extern const std::vector<EStatType> UpgradeableCoreStats;
}