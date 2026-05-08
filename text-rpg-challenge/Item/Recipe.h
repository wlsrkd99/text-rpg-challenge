#pragma once
#include "ItemBase.h"
#include <string>
#include <vector>

namespace TextRPG
{
	class Recipe : public ItemBase
	{
	private:
		int m_ResultItemID;
		int m_ResultItemCount;
		std::vector<std::pair<int, int>> m_Ingredients;	// 재료는 (아이템ID, 개수)
		std::string m_RecipeName;

	public:
		Recipe(int id, const std::string& name, int resultItemID, int resultItemCount, const std::vector<std::pair<int, int>>& ingredients)
			: ItemBase(id, name, EItemType::IT_RECIPE, EItemGrade::IG_NONE, 0, 1), m_ResultItemID(resultItemID), m_ResultItemCount(resultItemCount), m_Ingredients(ingredients), m_RecipeName(name) {
		}
		
		int GetResultItemID() const { return m_ResultItemID; }
		int GetResultItemCount() const { return m_ResultItemCount; }
		const std::string& GetRecipeName() const { return m_RecipeName; }

		const std::vector<std::pair<int, int>>& GetIngredients() const { return m_Ingredients; }
	};
}
