#pragma once

#include "../Data/BaseStatBonus.h"
#include "../Enums/EStatType.h"
#include "../Enums/EJobType.h"
#include <string>

namespace TextRPG
{
    // 기본 스탯에 따른 처리 보여줌
    std::string GetBonusDisplayString(const BaseStatBonus& bonus);
    // 직업타입에 따른 스탯 보여주기
    std::string GetStatNameByJobType(EJobType type);
    // 스탯타입에 따른 스탯 보여주기
	std::string GetStatNameByStatType(EStatType type);
}