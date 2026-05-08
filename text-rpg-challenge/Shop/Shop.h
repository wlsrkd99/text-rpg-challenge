#pragma once
#include "../Item/Inventory.h"
#include "../Item/ItemBase.h"
#include <vector>
namespace TextRPG
{
	class Shop
	{
	protected:
		Inventory m_Stock;

	public:
		Shop() = default;
		~Shop() = default;
		
		Inventory& GetStock() { return m_Stock; }

		/**
		 *  @brief 상점의 재고에 아이템을 추가합니다.
		 *  @param item 추가할 아이템 객체의 포인터
		 */
		void AddItemToStock(ItemBase* item) { m_Stock.AddItem(item); }
		void AddItemToStock(int itemID, int count);
		void AddItemToStock(const std::vector<ItemBase*>& items);
		

		/**
		 *  @brief 아이템을 판매할때 호출되는 함수입니다.
		 *  @param itemID 아이템의 ID
		 *  @param count 아이템의 개수
		 *  @param inven 플레이어의 인벤토리 참조
		 */
		int SellItem(int itemID, int count, Inventory& inven);

		/**
		 *  @brief 아이템을 살때 호출되는 함수입니다.
		 *  @param itemID 아이템의 ID
		 *  @param count 아이템의 개수
		 *  @param inven 플레이어의 인벤토리 참조
		 */
		bool BuyItem(int itemID, int count, Inventory& inven);

	};
}

