#pragma once

#include <string>
namespace TextRPG
{
	enum class EJobType 
	{
		JT_NOVICE,
		JT_WARRIOR,
		JT_MAGE,
		JT_ROGUE,
		JT_ARCHER,
		JT_MAX
	};

	inline std::string GetJobName(EJobType job)
	{
		switch (job)
		{
		case EJobType::JT_NOVICE:  	return "NOVICE";
		case EJobType::JT_WARRIOR: 	return "WARRIOR";
		case EJobType::JT_MAGE:    	return "MAGE";
		case EJobType::JT_ROGUE:   	return "ROGUE";
		case EJobType::JT_ARCHER:  	return "ARCHER";
		default:                 	return "UNKNOWN";
		}
	}
}
