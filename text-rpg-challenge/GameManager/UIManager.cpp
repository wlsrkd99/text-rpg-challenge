#include <iostream>
#include <iomanip>

#include "UIManager.h"
#include "../Data/StatData.h"
#include "../Enums/EStatType.h"
#include "../Unit/Player/Player.h"
#include "../Enums/EJobType.h"
#include "../Utils/StatFormatting.h"

namespace TextRPG
{
	void UIManager::PrintTitle(const std::string& title)
	{
		std::cout << "=============================================" << std::endl;
		std::cout << std::setw(45) << std::left << title << std::endl; // 타이틀 정렬
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

#pragma region Game Data Display
	void UIManager::PrintPlayerStats(const Player& player)
	{
		const int LABEL_WIDTH = 10;
		const int VALUE_WIDTH = 12;
		const std::string SEPARATOR((LABEL_WIDTH + VALUE_WIDTH) * 2, '=');

		PrintMessage(SEPARATOR);
		PrintMessage(" " + player.GetName() + "'s Stats");
		PrintMessage(SEPARATOR);


		std::string jobName = GetJobName(player.GetJob());
		PrintMessage("Job: " + jobName + " | Lv." + std::to_string(player.GetLevel()) + " (" + std::to_string(player.GetExp()) + " EXP)");
		
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

		for (int i = 0; i < static_cast<int>(EStatType::ST_Count); ++i)
		{
			EStatType baseStatType = static_cast<EStatType>(i);
			std::string baseStatName = GetStatName(baseStatType) + ":";
			std::string effectsString = GetStatNameByStatType(baseStatType);
			std::cout << std::left << std::setw(STAT_NAME_WIDTH) << baseStatName << effectsString << std::endl;
		}
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
#pragma endregion

#pragma region User Interaction Flow Control
	EJobType UIManager::ShowClassChangeMenuAndGetChoice(const Player& player)
	{
		PrintTitle("< Job Selection >");
		PrintMessage(player.GetName() + ", choose your job!");

		std::string jobSelectionText;
		for (int i = static_cast<int>(EJobType::JT_WARRIOR); i < static_cast<int>(EJobType::JT_MAX); i++)
		{
			jobSelectionText += std::to_string(i) + ". " + GetJobName(static_cast<EJobType>(i)) + "   ";
		}
		PrintMessage(jobSelectionText);

		int choice = 0;
		GetInputs("Choose: ", choice);
		if (choice < static_cast<int>(EJobType::JT_WARRIOR) || choice >= static_cast<int>(EJobType::JT_MAX))
		{
			PrintMessage("Invalid choice. You remain a Novice for now.");
			return EJobType::JT_NOVICE;
		}

		return static_cast<EJobType>(choice);
	}

	void UIManager::_handleHealthManaDistributionUI(std::array<int, static_cast<int>(EStatType::ST_Count)>& outBaseStats)
	{
		PrintMessage("\n--- Stat Distribution: Health & Mana ---");
		PrintMessage("Distribute a total of 100 points between Health and Mana.");
		while (true)
		{
			GetInputs("Enter Base Stats (Health Mana): ",
				outBaseStats[static_cast<int>(EStatType::ST_Health)],
				outBaseStats[static_cast<int>(EStatType::ST_Mana)]);

			if (outBaseStats[static_cast<int>(EStatType::ST_Health)] + outBaseStats[static_cast<int>(EStatType::ST_Mana)] == 100) {
				if (outBaseStats[static_cast<int>(EStatType::ST_Health)] > 10 && outBaseStats[static_cast<int>(EStatType::ST_Mana)] > 10) {
					break;
				}
				PrintMessage("Health and Mana must be greater than 10. Please try again.");
			}
			else {
				PrintMessage("The sum of Health and Mana must be exactly 100. Please try again.");
			}
		}
	}
	
	void UIManager::_handleCoreStatDistributionUI(std::array<int, static_cast<int>(EStatType::ST_Count)>& outBaseStats)
	{
		PrintMessage("\n--- Stat Distribution: Core Attributes ---");
		PrintMessage("Distribute a total of 20 points among the following stats.");
		int remainingPoints = 20;
		ShowStatDistribution(
			[&]() { return remainingPoints; },
			[&](int amount) {
				if (amount > remainingPoints) return false;
				remainingPoints -= amount;
				return true;
			},
			[&]() -> const auto& { return outBaseStats; },
			[&](EStatType stat, int amount) {
				outBaseStats[static_cast<int>(stat)] += amount;
			},
			nullptr
		);
		PrintMessage("\nAll initial stat points have been distributed!");
		PrintMessage("");
	}

	std::array<int, static_cast<int>(EStatType::ST_Count)> UIManager::PerformInitialStatDistribution()
	{
		std::array<int, static_cast<int>(EStatType::ST_Count)> baseStats;
		baseStats.fill(0);

		_handleHealthManaDistributionUI(baseStats); // Health/Mana 분배 UI 처리
		_handleCoreStatDistributionUI(baseStats);  // 핵심 스탯 분배 UI 처리

		return baseStats;
	}

	void UIManager::ShowStatDistribution(
		std::function<int()> getPoints,
		std::function<bool(int)> spendPoints,
		std::function<const std::array<int, static_cast<int>(EStatType::ST_Count)>& ()> getStatsArray,
		std::function<void(EStatType, int)> applyStatChange,
		std::function<void()> onUpdate)
	{
		const auto& coreStatTypes = UpgradeableCoreStats;

		std::vector<std::string> coreStatNames;
		coreStatNames.reserve(coreStatTypes.size());
		for (EStatType statType : coreStatTypes)
		{
			coreStatNames.push_back(GetStatName(statType));
		}

		while (getPoints() > 0)
		{
			PrintStatDistributionUI(coreStatNames, getStatsArray(), getPoints());

			int choice = 0;
			GetInputs("Choose a stat to upgrade (1-5, or 0 to exit): ", choice);
			if (choice == 0) break;
			if (choice < 1 || choice > 5) {
				PrintMessage("Invalid choice. Please select a number between 1 and 5.");
				continue;
			}

			int amount = 0;
			GetInputs("How many points to add?: ", amount);
			if (amount <= 0) {
				PrintMessage("You must add a positive number of points.");
				continue;
			}

			if (!spendPoints(amount)) {
				PrintMessage("You don't have enough points. You only have " + std::to_string(getPoints()) + " left.");
				continue;
			}

			EStatType statToUpgrade = coreStatTypes[choice - 1];
			applyStatChange(statToUpgrade, amount);
			PrintMessage(coreStatNames[choice - 1] + " increased by " + std::to_string(amount) + ".");

			if (onUpdate) {
				onUpdate();
			}
		}
		PrintMessage("\nFinished stat distribution.");
	}
#pragma endregion
}
