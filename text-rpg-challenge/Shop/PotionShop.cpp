#include "PotionShop.h"

namespace TextRPG
{
	void PotionShop::DispenseHpPotion(Inventory& inven)
	{
		int count = m_Stock.GetItemCount(101);
		if (count > 0)
		{
			inven.AddItem(new ItemBase(*m_Stock.FindItem(101)));
			m_Stock.RemoveItem(101, count);
		}
	}

	void PotionShop::ReturnPotion(Inventory& inven)
	{
		int count = inven.GetItemCount(100);
		if (count > 0)
		{
			inven.RemoveItem(100, count);
			AddItemToStock(101, count);
		}
	}

	bool PotionShop::ExchangeEmptyPotion(int count, Inventory& inven)
	{
		if (count <= 0) return false;
		
		if (inven.GetItemCount(100) >= count)
		{
			inven.RemoveItem(100, count);
			ItemBase* hpTemplate = m_Stock.FindItem(101);
			if (hpTemplate)
			{
				ItemBase* newHp = hpTemplate->Clone();
				newHp->SetCount(count);
				inven.AddItem(newHp);
				return true;
			}
		}
		return false;
	}
}