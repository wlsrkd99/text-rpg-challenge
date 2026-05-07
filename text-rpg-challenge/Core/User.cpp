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
                potion->Use(target);
                m_Inventory.RemoveItem(itemID, 1);
                result.bSuccess = true;
            }
        }
        return result;
    }
}