#include <iostream>
#include <limits>
#include <iomanip>
#include <sstream>
#if __cplusplus >= 202002L
#include <format> // C++20 이상에서 지원
#endif

#include "UIManager.h"
#include "../Unit/Player/Player.h"

namespace TextRPG
{
	void UIManager::PrintTitle(const std::string& title)
	{
		std::cout << "=============================================" << std::endl;
		std::cout << title << std::endl;
		std::cout << "=============================================" << std::endl;
	}

	void UIManager::PrintMessage(const std::string& message)
	{
		std::cout << message << std::endl;
	}

	std::string UIManager::GetStringInput(const std::string& prompt)
	{
		std::cout << prompt;
		std::string input;
		std::getline(std::cin, input);
		return input;
	}

	void UIManager::PrintPlayerStats(const Player& player)
	{
		const int LABEL_WIDTH = 10;
		const int VALUE_WIDTH = 12;
		const std::string SEPARATOR((LABEL_WIDTH + VALUE_WIDTH) * 2, '=');

		std::cout << SEPARATOR << std::endl;
		std::cout << " " << player.GetName() << "'s Stats" << std::endl;
		std::cout << SEPARATOR << std::endl;

		// HP and MP
		std::string hp_display = std::to_string(player.GetCurrentHP()) + "/" + std::to_string(player.GetMaxHP());
		std::string mp_display = std::to_string(player.GetCurrentMP()) + "/" + std::to_string(player.GetMaxMP());

		std::cout << std::left << std::setw(LABEL_WIDTH) << " HP:"
			<< std::right << std::setw(VALUE_WIDTH) << hp_display
			<< std::left << std::setw(LABEL_WIDTH) << "  MP:"
			<< std::right << std::setw(VALUE_WIDTH) << mp_display << std::endl;

		// Physical Stats
		std::cout << std::left << std::setw(LABEL_WIDTH) << " P. Atk:"
			<< std::right << std::setw(VALUE_WIDTH) << player.GetPAtk()
			<< std::left << std::setw(LABEL_WIDTH) << "  P. Def:"
			<< std::right << std::setw(VALUE_WIDTH) << player.GetPDef() << std::endl;

		// Magical Stats
		std::cout << std::left << std::setw(LABEL_WIDTH) << " M. Atk:"
			<< std::right << std::setw(VALUE_WIDTH) << player.GetMAtk()
			<< std::left << std::setw(LABEL_WIDTH) << "  M. Def:"
			<< std::right << std::setw(VALUE_WIDTH) << player.GetMDef() << std::endl;

		// Speed
		std::cout << std::left << std::setw(LABEL_WIDTH) << " Speed:"
			<< std::right << std::setw(VALUE_WIDTH) << player.GetSpeed() << std::endl;

		std::cout << SEPARATOR << std::endl;
	}
	
	void UIManager::PrintUpgradeMenu()
	{
		std::cout << "============================================" << std::endl;
		std::cout << "< Character Upgrade >" << std::endl;
		std::cout << "1. HP UP    2. MP UP    3. Attack x2" << std::endl;
		std::cout << "4. Defense x2  5. Show Stats  0. Start Game" << std::endl;
		std::cout << "============================================" << std::endl;
	}
}
