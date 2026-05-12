#include "Inventory.h"
#include "ItemBase.h"

namespace TextRPG
{
	Inventory::~Inventory()
	{
		for (auto pair : m_Items)
		{
			delete pair.second;
		}
		m_Items.clear();
	}

	bool Inventory::SpendMoney(int amount)
	{
		if (m_Gold >= amount)
		{
			m_Gold -= amount;
			return true;
		}
		return false;
	}


	void Inventory::AddItem(ItemBase* item)
	{
		if (item == nullptr) return;

		auto it = m_Items.find(item->GetID());
		if (it != m_Items.end())
		{
			it->second->SetCount(it->second->GetCount() + item->GetCount());
			delete item;
		}
		else
		{
			m_Items[item->GetID()] = item;
		}
	}
	void Inventory::AddItem(int itemID, int count)
	{
		auto it = m_Items.find(itemID);
		if (it != m_Items.end())
		{
			it->second->SetCount(it->second->GetCount() + count);
		}
	}

	bool Inventory::RemoveItem(int itemID, int count)
	{
		auto it = m_Items.find(itemID);
		if (it == m_Items.end())
		{
			return false;
		}

		ItemBase* item = it->second;
		if (item->GetCount() < count)
		{
			return false;
		}

		item->SetCount(item->GetCount() - count);

		if (item->GetCount() <= 0)
		{
			delete item;
			m_Items.erase(it);
		}
		return true;
	}

	int Inventory::GetItemCount(int itemID) const
	{
		auto it = m_Items.find(itemID);
		if (it != m_Items.end())
		{
			return it->second->GetCount();
		}
		return 0;
	}

	ItemBase* Inventory::FindItem(int itemID) const
	{
		auto it = m_Items.find(itemID);
		if (it != m_Items.end())
		{
			return it->second;
		}
		return nullptr;
	}
}
