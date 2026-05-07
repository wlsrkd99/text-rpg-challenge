#pragma once

#include "../Enums/EItemType.h"
#include "../Enums/EItemGrade.h"
#include <string>

namespace TextRPG
{
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
		int GetID() const { return m_ID; }
		const std::string& GetName() const { return m_Name; }
		EItemType GetType() const { return m_Type; }
		int GetPrice() const { return m_Price; }
		void SetPrice(int price) { m_Price = price; }
		int GetCount() const { return m_Count; }
		void AddCount(int count) { m_Count += count; }
	};
}
