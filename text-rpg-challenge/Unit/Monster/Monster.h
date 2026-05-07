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
		Monster(const std::string& name = "Monster");
		~Monster();
		
		long GetDropExp() const { return m_DropExp; }
		void SetDropExp(long exp) { m_DropExp = exp; }
		int GetDropGold() const { return m_DropGold; }
		void SetDropGold(int gold) { m_DropGold = gold; }
		ItemBase* GetDropItem() const { return m_DropItem; }
		/**
		 * @brief 몬스터가 드롭 아이템의 소유권을 포기하고 포인터를 반환합니다.
		 *        이후 몬스터는 해당 아이템을 해제하지 않습니다.
		 */
		ItemBase* ReleaseDropItem();
		void SetDropItem(ItemBase* item) { m_DropItem = item; }
	};
}
