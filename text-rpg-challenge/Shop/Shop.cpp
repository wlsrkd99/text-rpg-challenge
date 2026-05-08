#include "Shop.h"

namespace TextRPG
{
	void Shop::AddItemToStock(int itemID, int count=1)
	{
		if(m_Stock.FindItem(itemID))
		{
			m_Stock.AddItem(itemID, count);
		}
	}
	void Shop::AddItemToStock(const std::vector<ItemBase*>& items)
	{
		for (ItemBase* item : items)
		{
			AddItemToStock(item);
		}
	}

	int Shop::SellItem(int itemID, int count, Inventory& inven)
	{
		ItemBase* item = inven.FindItem(itemID);
		if (!item || item->GetCount() < count) return -1;
		
		int earnedGold = item->GetPrice() * count;
		if (inven.RemoveItem(itemID, count))
		{
			inven.GainGold(earnedGold);
			return earnedGold;
		}
		return -1;
	}
	
	bool Shop::BuyItem(int itemID, int count, Inventory& inven)
	{
		ItemBase* item = m_Stock.FindItem(itemID);
		if (!item) return false;

		if (inven.SpendMoney(item->GetPrice() * count))
		{
			ItemBase* cloned = item->Clone();
			cloned->SetCount(count);
			inven.AddItem(cloned);
			return true;
		}
		return false;
	}
}
