#pragma once
#include "Shop.h"
#include "../Item/Recipe.h"
#include <vector>


namespace TextRPG
{
    class PotionShop : public Shop
    {
    private:
		const int MAX_HP_POTION_COUNT = 5; 
        std::vector<Recipe> m_Recipes;

    public:
        /**
         * @brief HP 포션을 인벤토리에 추가합니다.
         * @param inven 플레이어의 인벤토리 참조
         */
        void DispenseHpPotion(Inventory& inven);
        /**
         * @brief 포션을 반환합니다.
         * @param inven 플레이어의 인벤토리 참조
         */
        void ReturnPotion(Inventory& inven);

        /**
         * @brief 빈 포션을 반환하고 HP 포션으로 교환합니다.
         * @param count 반환할 빈 포션 갯수
         * @param inven 플레이어 인벤토리
         */
        bool ExchangeEmptyPotion(int count, Inventory& inven);

        /**
         * @brief 레시피를 추가합니다.
         * @param recipe 추가할 레시피
         */
		void AddRecipe(const Recipe& recipe) { m_Recipes.push_back(recipe); }

        /**
         * @brief 모든 레시피를 출력합니다.
		 */
		std::vector<Recipe> ShowAllRecipe() const { return m_Recipes; }
        /**
         * @brief 이름으로 레시피를 검색합니다.
         * @param name 검색할 레시피 이름
         */
        std::vector<Recipe> SearchRecipeByName(const std::string& name) const;
        /**
         * @brief 재료로 레시피를 검색합니다.
         * @param ingredient 검색할 재료 이름
		 */
        std::vector<Recipe> SearchRecipeByIngredient(const std::string& ingredient) const;

        /**
         * @brief 레시피를 제작할 수 있는지 확인합니다.
         * @param recipe 확인할 레시피
         * @param inven 플레이어 인벤토리
		 * @return 제작 가능 여부
		 */
		bool CanCraft(const Recipe& recipe, const Inventory& inven) const;

        /**
         * @brief 레시피를 제작합니다.
         * @param recipe 제작할 레시피
         * @param inven 플레이어 인벤토리
         * @return 제작 성공 여부
		 */
		bool CraftItem(const Recipe& recipe, Inventory& inven);
    };
}
