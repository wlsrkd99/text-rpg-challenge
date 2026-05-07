#include <iostream>
#include <iomanip>
#include <string>

#include "UIManager.h"
#include "../Data/StatData.h"
#include "../Enums/EStatType.h"
#include "../Unit/Player/Player.h"
#include "../Enums/EJobType.h"
#include "../Data/BattleData.h"
#include "../Item/Inventory.h"
#include "../Utils/StatFormatting.h"

namespace TextRPG
{
#pragma region Basic I/O
	void UIManager::PrintTitle(const std::string& title)
	{
		std::cout << std::endl;
		std::cout << "=============================================" << std::endl;
		std::cout << std::setw(45) << std::left << title << std::endl;
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

	void UIManager::PrintGuide(const std::string& title, const std::string& description)
	{
		PrintMessage("\n--- " + title + " ---");
		if (!description.empty())
		{
			PrintMessage(description);
		}
	}

	int UIManager::PromptMenu(const MenuConfig& config)
	{
		if (!config.Title.empty()) PrintMessage("\n--- " + config.Title + " ---");
		for (const auto& info : config.Infos) PrintMessage(info);
		for (const auto& opt : config.Options) PrintMessage(opt);
		
		int choice = 0;
		GetInputs(config.Prompt, choice);
		return choice;
	}

	void UIManager::PromptCharacterName()
	{
		PrintTitle("[ Dungeon Escape Text RPG ]");
		std::string name = GetStringInput("Enter your hero's name: ");
		OnCharacterNameEntered.Broadcast(name);
	}
#pragma endregion

	void UIManager::PromptMainMenuAction(int gold)
	{
		PrintTitle("[ Main Menu ]");
		MenuConfig config;
		config.Title = "Where would you like to go?";
		config.Infos = { "Current Gold: " + std::to_string(gold) + "G" };
		config.Options = {
			"1. Go to Town",
			"2. Go to Dungeon (Battle)",
			"3. Upgrade Stats",
			"4. View Inventory",
			"0. Exit Game"
		};
		int choice = PromptMenu(config);
		OnMenuNavigationRequested.Broadcast(choice);
	}

#pragma region Stat
	namespace
	{
		constexpr int LABEL_WIDTH = 10;
		constexpr int VALUE_WIDTH = 12;
		const std::string SEPARATOR((LABEL_WIDTH + VALUE_WIDTH) * 2, '=');
	}

	void UIManager::DisplayCharacterSheet(const Player& player)
	{
		PrintTitle(" " + player.GetName() + "'s Stats");
		std::string jobName = GetJobName(player.GetJob());
		PrintMessage(" Job: " + jobName + " | Lv." + std::to_string(player.GetLevel()) + " (EXP " + std::to_string(player.GetExp()) + "/" + std::to_string(player.GetExpToNextLevel()) + ")");
		_handleBaseStatSheet(player);
		_handleDerivedStatSheet(player);
		PrintMessage(SEPARATOR);
	}
	void UIManager::_handleBaseStatSheet(const Player& player)
	{
		std::cout << std::left << std::setw(LABEL_WIDTH) << (" " + GetStatAlias(EStatType::ST_Endurance) + ":")
			<< std::right << std::setw(VALUE_WIDTH) << player.GetBaseStat(EStatType::ST_Endurance)
			<< std::left << std::setw(LABEL_WIDTH) << ("  " + GetStatAlias(EStatType::ST_Strength) + ":")
			<< std::right << std::setw(VALUE_WIDTH) << player.GetBaseStat(EStatType::ST_Strength) << std::endl;

		std::cout << std::left << std::setw(LABEL_WIDTH) << (" " + GetStatAlias(EStatType::ST_Agility) + ":")
			<< std::right << std::setw(VALUE_WIDTH) << player.GetBaseStat(EStatType::ST_Agility)
			<< std::left << std::setw(LABEL_WIDTH) << ("  " + GetStatAlias(EStatType::ST_Intelligence) + ":")
			<< std::right << std::setw(VALUE_WIDTH) << player.GetBaseStat(EStatType::ST_Intelligence) << std::endl;

		std::cout << std::left << std::setw(LABEL_WIDTH) << (" " + GetStatAlias(EStatType::ST_Wisdom) + ":")
			<< std::right << std::setw(VALUE_WIDTH) << player.GetBaseStat(EStatType::ST_Wisdom) << std::endl;
	}
	void UIManager::_handleDerivedStatSheet(const Player& player)
	{		
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
	}

	void UIManager::DisplayStatEffects()
	{
		PrintTitle("Base Stat Effects ");
		const int STAT_NAME_WIDTH = 18;
		for (int i = 0; i < static_cast<int>(EStatType::ST_Count); ++i)
		{
			EStatType baseStatType = static_cast<EStatType>(i);
			std::string baseStatName = GetStatName(baseStatType) + ":";
			std::string effectsString = GetStatNameByStatType(baseStatType);
			std::cout << std::left << std::setw(STAT_NAME_WIDTH) << baseStatName << effectsString << std::endl;
		}
		PrintMessage(SEPARATOR);
	}

	void UIManager::DisplayStatAllocation(const std::vector<std::string>& statNames, const std::array<int, static_cast<int>(EStatType::ST_Count)>& baseStats, int remainingPoints)
	{
		PrintMessage("");
		PrintMessage("------------------------------------");
		for (size_t i = 0; i < statNames.size(); ++i) {
			PrintMessage(std::to_string(i + 1) + ". " + statNames[i] + ": " + std::to_string(baseStats[static_cast<int>(EStatType::ST_Endurance) + i]));
		}
		PrintMessage("------------------------------------");
		PrintMessage("Points left: " + std::to_string(remainingPoints));
	}

	void UIManager::PromptStatAllocation(const std::vector<std::string>& statNames, const std::array<int, static_cast<int>(EStatType::ST_Count)>& baseStats, int remainingPoints)
	{ 
		DisplayStatAllocation(statNames, baseStats, remainingPoints);
		
		int choice = 0;
		std::string maxChoice = std::to_string(statNames.size());
		GetInputs("Choose a stat to upgrade (1-" + maxChoice + ", or 0 to exit): ", choice);
		if (choice == 0) {
			OnStatUpgradeRequested.Broadcast(EStatType::ST_Count, 0);
			return;
		}
		if (choice < 1 || choice > statNames.size()) {
			PrintMessage("Invalid choice. Please select a number between 1 and " + maxChoice + ".");
			return;
		}

		int amount = 0;
		GetInputs("How many points to add?: ", amount);
		if (amount <= 0) {
			PrintMessage("You must add a positive number of points.");
			return;
		}

		EStatType statToUpgrade = UpgradeableCoreStats[choice - 1];
		OnStatUpgradeRequested.Broadcast(statToUpgrade, amount);
	}

	void UIManager::PromptInitialStatAllocation()
	{
		PrintGuide("Initialize Stat: Health & Mana", "Distribute a total of 100 points between Health and Mana.");
		while (true)
		{
			int hp = 0;
			int mp = 0;
			GetInputs("Enter Base Stats (Health Mana): ", hp, mp);

			if (hp + mp == 100) {
				if (hp > 10 && mp > 10) {
					OnInitialStatAllocated.Broadcast(hp, mp);
					break;
				}
				PrintMessage("Health and Mana must be greater than 10. Please try again.");
			}
			else {
				PrintMessage("The sum of Health and Mana must be exactly 100. Please try again.");
			}
		}
	}

	void UIManager::DisplayDefaultItem(const std::vector<std::pair<std::string, int>>& receivedItems)
	{
		PrintTitle("Default Items");
		for (const auto& item : receivedItems)
		{
			std::cout << "You received " << item.first << " " << item.second << " !!" << std::endl;
		}
		PrintMessage("============================================");
	}
#pragma endregion

#pragma region Battle
	void UIManager::PromptBattleAction()
	{
		MenuConfig config;
		config.Title = "Player Turn";
		config.Options = { "1. Attack", "2. Skill", "3. Use Item" };
		int choice = PromptMenu(config);

		switch (choice)
		{
		case 1: OnBattleActionSelected.Broadcast(EBattleActionType::BAT_ATTACK); break;
		case 2: OnBattleActionSelected.Broadcast(EBattleActionType::BAT_SKILL); break;
		case 3: OnBattleActionSelected.Broadcast(EBattleActionType::BAT_ITEM); break;
		default:
			PrintMessage("Invalid choice. Please try again.");
			break;
		}
	}

	void UIManager::DisplayAttackResult(const AttackResult& result)
	{
		PrintMessage("[" + result.AttackerName + "] " + result.AttackName);
		for (int i = 0; i < result.Damages.size(); i++)
		{
			PrintMessage(std::to_string(result.Damages[i]) + " damage to " + result.TargetName + "!");
			std::string hpMessage = result.TargetName + " HP: " + std::to_string(result.TargetHP + result.Damages[i]) + " -> " + std::to_string(result.TargetHP);
			if (result.TargetHP <= 0)
			{
				hpMessage += " (Dead)";
			}
			PrintMessage(hpMessage);
		}
	}

	void UIManager::DisplayBattleWinDetails(const BattleWinData& data)
	{
		PrintMessage("=== VICTORY ===");
		PrintMessage("  -> Get " + std::to_string(data.EarnedExp) + " EXP and " + std::to_string(data.EarnedGold) + " Gold!");
		for (int i = 0; i < data.Rewards.size(); i++)
		{
			PrintMessage("  -> Get: " + data.Rewards[i]->GetName() + " +" + std::to_string(data.Rewards[i]->GetCount()));
		}
		PrintMessage("(Will be saved to inventory in the next STEP)");
	}

	void UIManager::DisplayBattleLoseDetails(const BattleLoseData& data)
	{
		PrintMessage("=== DEFEAT ===");
		PrintMessage("Don't give up! Try again and defeat your enemies!");
	}


#pragma endregion

#pragma region Inventory
	void UIManager::DisplayInventory(const Inventory& inventory)
	{
		PrintTitle("[ Inventory ]");
		if (inventory.GetAllItems().empty())
		{
			PrintMessage("Inventory is empty.");
			return;
		}

		int i = 1;
		for (const auto& pair : inventory.GetAllItems())
		{
			PrintMessage(std::to_string(i++) + ". " + pair.second->GetName() + " +" + std::to_string(pair.second->GetCount()) + "(" + std::to_string(pair.second->GetPrice()) + "G)");
		}
	}

	void UIManager::DisplayItemUseResult(const ItemUseResult& result)
	{
		if (!result.bSuccess)
		{
			PrintMessage("Cannot use that item.");
			return;
		}

		PrintMessage("* " + result.ItemName + " used!");
		
		for (const auto& status : result.CuredStatus)
			PrintMessage("  -> " + result.TargetName + " is cured of [" + status + "]!");

		for (const auto& status : result.AppliedStatus)
			PrintMessage("  -> " + result.TargetName + " is inflicted with [" + status + "]!");


		if (result.RestoredHP > 0)
			PrintMessage("  -> " + result.TargetName + "'s HP restored by " + std::to_string(result.RestoredHP) + ".");
		if (result.RestoredMP > 0)
			PrintMessage("  -> " + result.TargetName + "'s MP restored by " + std::to_string(result.RestoredMP) + ".");
		if (result.Damage > 0)
			PrintMessage("  -> " + result.TargetName + " took " + std::to_string(result.Damage) + " damage!");
	}

	void UIManager::PromptItemUse(const Inventory& inventory)
	{
		while (true)
		{
			int choice = 0;
			GetInputs("Choose item: ", choice);
			if (choice == 0)
			{
				OnItemUseRequested.Broadcast(-1);
				break;
			}
			if (choice > 0 && choice <= inventory.GetAllItems().size())
			{
				auto it = inventory.GetAllItems().begin();
				std::advance(it, choice - 1);
				OnItemUseRequested.Broadcast(it->second->GetID());
				break;
			}
			PrintMessage("Invalid choice. Please try again.");
		}
	}
#pragma endregion

#pragma region Town
	void UIManager::PromptJobChange(const Player& player)
	{
		if (player.GetJob() != EJobType::JT_NOVICE)
		{
			PrintMessage("You already have a job.");
			return;
		}

		MenuConfig config;
		config.Title = "Job Selection";
		config.Infos = { player.GetName() + ", choose your job!" };
		for (int i = static_cast<int>(EJobType::JT_WARRIOR); i < static_cast<int>(EJobType::JT_MAX); i++)
		{
			config.Options.push_back(std::to_string(i) + ". " + GetJobName(static_cast<EJobType>(i)));
		}

		int choice = PromptMenu(config);

		if (choice < static_cast<int>(EJobType::JT_WARRIOR) || choice >= static_cast<int>(EJobType::JT_MAX))
		{
			PrintMessage("Invalid choice. You remain a Novice for now.");
			return;
		}

		EJobType selectedJob = static_cast<EJobType>(choice);
		OnJobChangeRequested.Broadcast(selectedJob);
	}
#pragma endregion
}
