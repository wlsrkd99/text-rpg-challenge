#pragma once
#include <string>

namespace TextRPG
{
    enum class EDerivedStatType
    {
        MaxHP,
        MaxMP,
        PAtk,
        PDef,
        MAtk,
        MDef,
        Speed
    };

    inline std::string GetDerivedStatName(EDerivedStatType stat)
    {
        switch (stat)
        {
        case EDerivedStatType::MaxHP:       return "MaxHP";
        case EDerivedStatType::MaxMP:       return "MaxMP";
        case EDerivedStatType::PAtk:        return "P.Atk";
        case EDerivedStatType::PDef:        return "P.Def";
        case EDerivedStatType::MAtk:        return "M.Atk";
        case EDerivedStatType::MDef:        return "M.Def";
        case EDerivedStatType::Speed:       return "Speed";
        default: return "Unknown";
        }
    }
}
