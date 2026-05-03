#include "Inventory.h"
#include "ItemBase.h"

namespace TextRPG
{
	Inventory::~Inventory()
	{
		for (auto const& pair : m_Items)
		{
			delete pair.second;
		}
		m_Items.clear();
	}

	void Inventory::AddItem(ItemBase* newItem)
	{
		if (!newItem) return;

		auto it = m_Items.find(newItem->GetID());
		if (it != m_Items.end())
		{
			it->second->AddCount(newItem->GetCount());
			delete newItem;
		}
		else
		{
			m_Items[newItem->GetID()] = newItem;
		}
	}

	bool Inventory::RemoveItem(int itemID, int count)
	{
		auto it = m_Items.find(itemID);
		if (it == m_Items.end() || it->second->GetCount() < count)
		{
			return false; // 아이템이 없거나 개수가 부족합니다.
		}

		it->second->AddCount(-count);
		if (it->second->GetCount() <= 0)
		{
			delete it->second;
			m_Items.erase(it);
		}
		return true;
	}

	int Inventory::GetItemCount(int itemID) const
	{
		auto it = m_Items.find(itemID);
		return (it != m_Items.end()) ? it->second->GetCount() : 0;
	}

	ItemBase* Inventory::FindItem(int itemID) const
	{
		auto it = m_Items.find(itemID);
		return (it != m_Items.end()) ? it->second : nullptr;
	}
}
