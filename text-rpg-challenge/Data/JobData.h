#pragma once

#include "../Enums/EJobType.h"
#include "BaseStatBonus.h"
#include <map>
#include <vector>
#include <string>

namespace TextRPG
{
    /**
     * 직업과 관련된 모든 정적 데이터를 책임지는 헤더
     */

    /** @brief 직업별 보너스 스탯 */
    extern const std::map<EJobType, std::vector<BaseStatBonus>> JobBonuses;

}