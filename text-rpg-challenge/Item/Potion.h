#pragma once
#include "ItemBase.h"
#include "../Enums/EItemType.h"
#include "../Enums/EItemValueType.h"
#include "../Data/BattleData.h"
#include "../Unit/Character.h"

namespace TextRPG
{
	class Potion : public ItemBase
	{
	private:
		ItemEffects m_Effects;
	public:
		/**
		 * @brief 포션 아이템을 생성합니다.
		 * @param id 아이템의 템플릿 ID
		 * @param name 아이템 이름
		 * @param effects 아이템의 효과 데이터
		 * @param price 아이템의 가격
		 * @param count 아이템 갯수
		 */
		Potion(int id, const std::string& name, const ItemEffects& effects, int price, int count = 1)
			: ItemBase(id, name, EItemType::IT_POTION, EItemGrade::IG_COMMON, price, count), m_Effects(effects) {}

		ItemUseResult Use(Character& target);

		virtual ItemBase* Clone() const override { return new Potion(*this); }
	};
}
