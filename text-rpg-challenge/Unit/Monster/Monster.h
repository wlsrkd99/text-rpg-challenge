#pragma once

#include"../Character.h"
#include"../../Item/ItemBase.h"


namespace TextRPG
{
	class Monster : public Character
	{
	private:
		long m_DropExp;
		int m_DropGold;
		ItemBase* m_DropItem;

	public:
		Monster(const std::string& name = "Monster") : Character(name)
		{
			m_DropExp = 0;
			m_DropGold = 0;
			m_DropItem = nullptr;
		}
		long GetDropExp() const { return m_DropExp; }
		void SetDropExp(long exp) { m_DropExp = exp; }
		int GetDropGold() const { return m_DropGold; }
		void SetDropGold(int gold) { m_DropGold = gold; }
		ItemBase* GetDropItem() const { return m_DropItem; }
		void SetDropItem(ItemBase* item) { m_DropItem = item; }
	};
}
