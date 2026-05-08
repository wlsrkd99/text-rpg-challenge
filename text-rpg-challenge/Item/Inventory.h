#pragma once
#include <string>
#include <map>


namespace TextRPG
{
	class ItemBase; 
	enum class EItemType;

	/**
	 * @brief 아이템의 저장, 추가, 제거, 검색을 담당하는 인벤토리 클래스
	 */
	class Inventory
	{
	private:
		/** Key: 아이템 템플릿 ID, Value: 해당 아이템 스택을 나타내는 객체 */ 
		std::map<int, ItemBase*> m_Items;
		int m_Gold = 0;
	public:
		Inventory() = default;
		~Inventory();
		
		int GetGold() const { return m_Gold; }
		void GainGold(int amount) { m_Gold += amount; }
		bool SpendMoney(int amount);

		/** @brief 특정 ID의 아이템을 추가하거나 갯수를 증가시킵니다. */
		void AddItem(ItemBase* item);
		void AddItem(int itemID, int count);

		/** @brief 특정 ID의 아이템을 지정된 개수만큼 제거합니다. */
		bool RemoveItem(int itemID, int count);

		/** @brief 특정 ID의 아이템 개수를 반환합니다. */
		int GetItemCount(int itemID) const;
		/** @brief 특정 ID의 아이템 객체를 찾아서 반환합니다. */
		ItemBase* FindItem(int itemID) const;
		/** @brief 인벤토리의 모든 아이템 맵을 반환합니다. */
		const std::map<int, ItemBase*>& GetAllItems() const { return m_Items; }

		/** @brief 특정 타입의 아이템들만 필터링하여 반환합니다. */
		//const std::map<int, ItemBase*>& GetItemsByType(EItemType type) const;
	};
};
