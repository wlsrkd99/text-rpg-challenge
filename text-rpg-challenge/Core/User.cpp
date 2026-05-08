#include "User.h"
#include "../Item/Potion.h"

namespace TextRPG
{
    ItemUseResult User::UseItem(int itemID, Character& target)
    {
        ItemUseResult result;

        ItemBase* item = m_Inventory.FindItem(itemID);
        if (!item)
        {
            return result;
        }

        if (item->GetType() == EItemType::IT_POTION)
        {
            Potion* potion = static_cast<Potion*>(item);
            if (potion)
            {
                result.ItemName = potion->GetName();
                if (m_Inventory.RemoveItem(itemID, 1))
                {
                    result = potion->Use(target);
                    result.bSuccess = true;

                    m_Inventory.AddItem(new ItemBase(100, "Empty Potion", EItemType::IT_POTION, EItemGrade::IG_COMMON, 0, 1));
                }
            }
        }
        return result;
    }
}