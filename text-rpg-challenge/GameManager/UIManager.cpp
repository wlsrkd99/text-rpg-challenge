#include <iostream>
#include <iomanip>

#include "UIManager.h"
#include "../Unit/Player/Player.h"
#include "../Data/EStatType.h" // For EStatType in PrintStatDistributionUI

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

		PrintMessage(SEPARATOR);
		PrintMessage(" " + player.GetName() + "'s Stats");
		PrintMessage(SEPARATOR);

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

		// Available Stat Points
		std::cout << std::left << std::setw(LABEL_WIDTH) << " Points:"
			<< std::right << std::setw(VALUE_WIDTH) << player.GetAvailableStatPoints() << std::endl;

		PrintMessage(SEPARATOR);
	}

	void UIManager::PrintDefaultItem(const std::vector<std::pair<std::string, int>>& receivedItems)
	{
		PrintMessage("");
		PrintMessage("========You received Default Items!!========");
		for (const auto& item : receivedItems)
		{
			std::cout << "You received " << item.first << " " << item.second << " !!" << std::endl;
		}
		PrintMessage("============================================");
	}
	
	void UIManager::PrintUpgradeMenu(const Player& player)
	{
		PrintMessage("\n--- Character Upgrade Menu ---");
		PrintPlayerStats(player);
		PrintMessage("Available Stat Points: " + std::to_string(player.GetAvailableStatPoints()));
		PrintMessage("1. Show Stat Effects");
		PrintMessage("2. Upgrade Base Stats");
		PrintMessage("0. Exit Upgrade Menu");
		PrintMessage("============================================");
	}

	void UIManager::PrintStatEffects()
	{
		PrintMessage("\n--- Base Stat Effects ---");
		const int STAT_NAME_WIDTH = 18;
		std::cout << std::left << std::setw(STAT_NAME_WIDTH) << "Health:" << "Increases MaxHP (x5)" << std::endl;
		std::cout << std::left << std::setw(STAT_NAME_WIDTH) << "Mana:" << "Increases MaxMP (x5)" << std::endl;
		std::cout << std::left << std::setw(STAT_NAME_WIDTH) << "Endurance:" << "Increases MaxHP (x2), P.Def (x2)" << std::endl;
		std::cout << std::left << std::setw(STAT_NAME_WIDTH) << "Strength:" << "Increases MaxHP (x1), P.Atk (x2)" << std::endl;
		std::cout << std::left << std::setw(STAT_NAME_WIDTH) << "Agility:" << "Increases Speed (x1)" << std::endl;
		std::cout << std::left << std::setw(STAT_NAME_WIDTH) << "Intelligence:" << "Increases MaxMP (x2), M.Atk (x2)" << std::endl;
		std::cout << std::left << std::setw(STAT_NAME_WIDTH) << "Wisdom:" << "Increases MaxMP (x1), M.Def (x2)" << std::endl;
		PrintMessage("============================================");
	}

	void UIManager::PrintStatDistributionUI(const std::vector<std::string>& statNames, const std::array<int, static_cast<int>(EStatType::ST_Count)>& baseStats, int remainingPoints)
	{
		PrintMessage("\nPoints left: " + std::to_string(remainingPoints));
		PrintMessage("------------------------------------");
		for (size_t i = 0; i < statNames.size(); ++i) {
			PrintMessage(std::to_string(i + 1) + ". " + statNames[i] + ": " + std::to_string(baseStats[static_cast<int>(EStatType::ST_Endurance) + i]));
		}
		PrintMessage("------------------------------------");
	}
}
