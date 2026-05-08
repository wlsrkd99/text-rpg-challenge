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
	std::vector<Recipe> PotionShop::SearchRecipeByName(const std::string& name) const
	{
		std::vector<Recipe> result;
		for(Recipe recipe : m_Recipes)
		{
			if(recipe.GetName().find(name) != std::string::npos)
			{
				result.push_back(recipe);
			}
		}
		return result;
	}
	std::vector<Recipe> PotionShop::SearchRecipeByIngredient(const std::string& ingredient) const
	{
		std::vector<Recipe> result;
		for(Recipe recipe : m_Recipes)
		{
			for(const auto& it : recipe.GetIngredients())
			{
				//TODO : 아이템 ID -> 이름 매핑 필요
				ItemBase* item = m_Stock.FindItem(it.first);
				std::string itemName = item ? item->GetName() : (it.first == 100 ? "Empty Potion" : "Unknown Item");

				if(itemName.find(ingredient) != std::string::npos)
				{
					result.push_back(recipe);
					break;
				}
			}
		}
		return result;
	}
	bool PotionShop::CanCraft(const Recipe& recipe, const Inventory& inven) const
	{
		const std::vector<std::pair<int, int>>& ingredients = recipe.GetIngredients();
		for (const auto& it : ingredients)
		{
			int itemID = it.first;
			int count = it.second;
			if (inven.GetItemCount(itemID) < count)
			{
				return false;
			}
		}
		return true;
	}
	bool PotionShop::CraftItem(const Recipe& recipe, Inventory& inven)
	{
		if (!CanCraft(recipe, inven))
			return false;
		
		const std::vector<std::pair<int, int>>& ingredients = recipe.GetIngredients();
		for (const auto& it : ingredients)
		{
			int itemID = it.first;
			int count = it.second;
			inven.RemoveItem(itemID, count);
		}

		ItemBase* resultItem = m_Stock.FindItem(recipe.GetResultItemID());
		if (resultItem)
		{
			ItemBase* newItem = resultItem->Clone();
			newItem->SetCount(recipe.GetResultItemCount());
			inven.AddItem(newItem);
		}
		else
		{

		}
		return true;
	}
}