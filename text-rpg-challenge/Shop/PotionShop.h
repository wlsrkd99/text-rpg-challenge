#pragma once
#include "Shop.h"


namespace TextRPG
{
    class PotionShop : public Shop
    {
    private:
		const int MAX_HP_POTION_COUNT = 5; 
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

        //void ShowAllRecipe() const;
        //void SearchRecipeByName(const std::string& name) const;
        //void SearchRecipeByIngredient(const std::string& ingredient) const;
    };
}
