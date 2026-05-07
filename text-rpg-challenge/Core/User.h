#pragma once
#include "../Unit/Player/Player.h"
#include "../Item/Inventory.h"

namespace TextRPG
{
	class User
	{
	private:
		Player m_MainPlayer;
		Inventory m_Inventory;
		int m_Gold = 0;

	public:
		User(const std::string& name) : m_MainPlayer()
		{
			m_MainPlayer.SetName(name);
		}

		Player* GetPlayer() { return &m_MainPlayer; }
		Inventory& GetInventory() { return m_Inventory; }
		int GetGold() const { return m_Gold; }
		void AddGold(int gold) { m_Gold += gold; }

		ItemUseResult UseItem(int itemID, Character& target); 
	};
}
