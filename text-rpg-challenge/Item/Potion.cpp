#include "Potion.h"
#include "../Data/BattleData.h"

namespace TextRPG
{
    ItemUseResult Potion::Use(Character& target)
    {
        ItemUseResult result;
        result.bSuccess = true;
        result.ItemName = GetName();
        result.TargetName = target.GetName();

        if (m_Effects.RestoreHP.Value > 0)
        {
            int amount = m_Effects.RestoreHP.Value;
            if (m_Effects.RestoreHP.Type == EItemValueType::IVT_PERCENT)
            {
                amount = (target.GetMaxHP() * amount) / 100;
            }
            result.RestoredHP = target.RestoreHealth(amount);
        }
            
        if (m_Effects.RestoreMP.Value > 0)
        {
            int amount = m_Effects.RestoreMP.Value;
            if (m_Effects.RestoreMP.Type == EItemValueType::IVT_PERCENT)
            {
                amount = (target.GetMaxMP() * amount) / 100;
            }
            result.RestoredMP = target.RestoreMana(amount);
        }

        if (m_Effects.Damage.Value > 0)
        {
            int amount = m_Effects.Damage.Value;
            if (m_Effects.Damage.Type == EItemValueType::IVT_PERCENT)
            {
                // 데미지의 %는 보통 최대 체력에 비례합니다.
                amount = (target.GetMaxHP() * amount) / 100;
            }
            target.TakeDamage(amount);
            result.Damage = amount;
        }
        result.CuredStatus = m_Effects.CuredStatus;
        result.AppliedStatus = m_Effects.AppliedStatus;

        return result;
    }
}
