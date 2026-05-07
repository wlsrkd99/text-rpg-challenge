#pragma once

#include <string>
namespace TextRPG
{
	enum class EStatType
	{
		ST_Health,
		ST_Mana,
		ST_Endurance,
		ST_Strength,
		ST_Agility,
		ST_Intelligence,
		ST_Wisdom,
		ST_Count
	};

	inline std::string GetStatName(EStatType stat)
	{
		switch (stat)
		{
		case EStatType::ST_Health: 			return "Health";
		case EStatType::ST_Mana: 			return "Mana";
		case EStatType::ST_Endurance: 		return "Endurance";
		case EStatType::ST_Strength: 		return "Strength";
		case EStatType::ST_Agility: 		return "Agility";
		case EStatType::ST_Intelligence: 	return "Intelligence";
		case EStatType::ST_Wisdom: 			return "Wisdom";
		default: return "Unknown";
		}
	}

	inline std::string GetStatAlias(EStatType stat)
	{
		switch (stat)
		{
		case EStatType::ST_Health: 			return "Hp";
		case EStatType::ST_Mana: 			return "Mp";
		case EStatType::ST_Endurance: 		return "End";
		case EStatType::ST_Strength: 		return "Str";
		case EStatType::ST_Agility: 		return "Agi";
		case EStatType::ST_Intelligence: 	return "Int";
		case EStatType::ST_Wisdom: 			return "Wis";
		default: return "Unknown";
		}
	}
}
