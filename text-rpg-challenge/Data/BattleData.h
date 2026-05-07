#pragma once
#include "../Item/ItemBase.h"
#include <string>
#include <vector>

namespace TextRPG
{
    struct AttackResult
    {
        std::string AttackerName;
        std::string AttackName;
        std::string TargetName;
        std::vector<int> Damages;
        int TargetHP = 0;
    };

    struct BattleWinData
    {
        int EarnedExp = 0;
        int EarnedGold = 0;
        std::vector<ItemBase*> Rewards;
	};

    struct BattleLoseData
    {
        
	};

    struct ItemUseResult
    {
        bool bSuccess = false;
        std::string ItemName;
        std::string TargetName;
        std::vector<std::string> CuredStatus;
        std::vector<std::string> AppliedStatus;
        int Damage = 0;
        int RestoredHP = 0;
        int RestoredMP = 0;
    };
}
