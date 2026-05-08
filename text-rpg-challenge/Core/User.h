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

	public:
		User(const std::string& name) : m_MainPlayer()
		{
			m_MainPlayer.SetName(name);
		}

		Player* GetPlayer() { return &m_MainPlayer; }
		Inventory& GetInventory() { return m_Inventory; }

		ItemUseResult UseItem(int itemID, Character& target); 
	};
}
