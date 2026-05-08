#pragma once

#include "../Enums/EItemType.h"
#include "../Enums/EItemGrade.h"
#include "../Enums/EItemValueType.h"
#include <string>
#include <vector>

namespace TextRPG
{
	struct EffectValue
	{
		EItemValueType Type = EItemValueType::IVT_FLAT;
		int Value = 0;
	};
	struct ItemEffects
	{
		EffectValue RestoreHP;
		EffectValue RestoreMP;
		EffectValue Damage;
		std::vector<std::string> CuredStatus;
		std::vector<std::string> AppliedStatus;
	};

	class ItemBase
	{
	private:
		int m_ID = 0;
		std::string m_Name = "";
		EItemType m_Type = EItemType::IT_NONE;
		EItemGrade m_Grade = EItemGrade::IG_NONE;
		int m_Price = 0;
		int m_Count = 0;

	public:
		ItemBase(int id, const std::string& name, EItemType type, EItemGrade grade, int price,int count)
			: m_ID(id), m_Name(name), m_Type(type), m_Grade(grade), m_Price(price), m_Count(count) {
		}
		virtual ~ItemBase() = default;

		
		int GetID() const { return m_ID; }
		const std::string& GetName() const { return m_Name; }
		EItemType GetType() const { return m_Type; }
		int GetPrice() const { return m_Price; }
		void SetPrice(int price) { m_Price = price; }
		int GetCount() const { return m_Count; }
		void AddCount(int count) { m_Count += count; }
		void SetCount(int count) { m_Count = count; }

		// @brief 객체 복사 시 다형성을 유지하기 위한 가상 팩토리 메서드
		virtual ItemBase* Clone() const { return new ItemBase(*this); }
	};
}
