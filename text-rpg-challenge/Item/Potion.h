#pragma once
#include "ItemBase.h"
#include "../Enums/EItemType.h"
#include "../Enums/EItemValueType.h"
#include "../Unit/Character.h"

namespace TextRPG
{
	class Potion : public ItemBase
	{
	private:
		EItemValueType m_ValueType;
		int m_Value = 0;
	public:
		/**
		 * @brief 포션 아이템을 생성합니다.
		 * @param id 아이템의 템플릿 ID
		 * @param name 아이템 이름 (예: "체력 포션")
		 * @param valueType 회복 타입 (고정 수치 또는 비율)
		 * @param value 회복량
		 * @param count 아이템 갯수
		 */
		Potion(int id, const std::string& name, EItemValueType valueType, int value, int count = 1)
			: ItemBase(id, name, EItemType::IT_POTION, EItemGrade::IG_COMMON, count), m_ValueType(valueType), m_Value(value) {}

		void Drink(Character& target);
	};
}
