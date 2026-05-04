#include "StatFormatting.h"
#include "../Data/StatData.h"
#include "../Enums/EDerivedStatType.h"
#include "../Enums/EStatType.h"
#include "../Enums/EJobType.h"
#include "../Data/JobData.h"

namespace TextRPG
{
    namespace
    {
        std::string FormatDisplayString(const std::string& name, int value)
        {
            return name + " +" + std::to_string(value);
        }
    }

    std::string GetBonusDisplayString(const BaseStatBonus& bonus)
    {
        if (bonus.StatType == EStatType::ST_Mana || bonus.StatType == EStatType::ST_Health)
        {
            auto effectIt = BaseStatEffects.find(bonus.StatType);
            if (effectIt != BaseStatEffects.end() && !effectIt->second.empty())
            {
                const auto& primaryEffect = effectIt->second[0];
                std::string derivedStatName = (primaryEffect.DerivedStat == EDerivedStatType::MaxMP) ? "MP" : "HP";
                return FormatDisplayString(derivedStatName, bonus.Amount * primaryEffect.Multiplier);
            }
        }
        return FormatDisplayString(GetStatName(bonus.StatType), bonus.Amount);
    }

    std::string GetStatNameByJobType(EJobType type)
    {
        auto it = JobBonuses.find(type);
        if (it == JobBonuses.end() || it->second.empty())
        {
            return "";
        }

        std::string message = "";
        for (size_t i = 0; i < it->second.size(); ++i)
        {
            const auto& bonus = it->second[i];
            message += GetBonusDisplayString(bonus);

            if (i < it->second.size() - 1) message += ", ";
        }


        return message;
    }

    std::string GetStatNameByStatType(EStatType type)
    {
        auto it = BaseStatEffects.find(type);
        if (it == BaseStatEffects.end() || it->second.empty())
        {
            return "No effect";
        }

        std::string effectsString = "";
        for (size_t i = 0; i < it->second.size(); ++i)
        {
            const auto& effect = it->second[i];
            effectsString += FormatDisplayString(GetDerivedStatName(effect.DerivedStat), effect.Multiplier);
            if (i < it->second.size() - 1)
            {
                effectsString += ", ";
            }
        }
        return effectsString;
    }
}