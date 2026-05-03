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

	void UIManager::GetTwoIntsInput(const std::string& prompt, int& val1, int& val2)
	{
		while (true)
		{
			std::cout << prompt;
			std::cin >> val1 >> val2;
			if (std::cin.fail())
			{
				std::cout << "Invalid input. Please enter two integers." << std::endl;
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			}
			else
			{
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				break;
			}
		}
	}

	void UIManager::PrintPlayerStats(const Player& player)
	{
		// 1. 출력용 문자열 생성
		std::string hp_display = std::to_string(player.GetStat(EStatType::Hp)) + "/" + std::to_string(player.GetMaxHP());
		std::string mp_display = std::to_string(player.GetStat(EStatType::Mp)) + "/" + std::to_string(player.GetMaxMP());
		std::string atk_display = std::to_string(player.GetStat(EStatType::Atk));
		std::string def_display = std::to_string(player.GetStat(EStatType::Def));

#if __cplusplus >= 202002L
		std::cout << "========================================" << std::endl;
		std::cout << std::format(" {:<}'s Stats\n", player.GetName());
		std::cout << "========================================" << std::endl;
		std::cout << std::format("{:<10}{:>10} {:<10}{:>10}\n", " HP:", hp_display, " MP : ", mp_display);
		std::cout << std::format("{:<10}{:>10} {:<10}{:>10}\n", " Attack:", atk_display, " Defense:", def_display);
		std::cout << "========================================" << std::endl;
#else
		// 2. 가장 긴 데이터의 길이를 찾아 너비 결정
		size_t max_val_len = std::max({ hp_display.length(), mp_display.length(), atk_display.length(), def_display.length(), (size_t)10 });

		int label_width = 10;
		int value_width = static_cast<int>(max_val_len) + 2;
		int total_line_width = (label_width + value_width) * 2;

		// 3. 출력
		std::string separator(total_line_width, '=');
		std::cout << separator << std::endl;
		std::cout << " " << player.GetName() << "'s Stats" << std::endl;
		std::cout << separator << std::endl;

		// HP & MP
		std::cout << std::left << std::setw(label_width) << " HP:"
			<< std::right << std::setw(value_width) << hp_display
			<< std::left << std::setw(label_width) << "  MP:"
			<< std::right << std::setw(value_width) << mp_display << std::endl;

		// ATK & DEF
		std::cout << std::left << std::setw(label_width) << " Attack:"
			<< std::right << std::setw(value_width) << atk_display
			<< std::left << std::setw(label_width) << "  Defense:"
			<< std::right << std::setw(value_width) << def_display << std::endl;
		std::cout << separator << std::endl;
#endif
	}

	int UIManager::GetIntegerInput(const std::string& prompt)
	{
		int choice;
		while (true)
		{
			std::cout << prompt;
			std::cin >> choice;
			if (std::cin.fail())
			{
				std::cout << "잘못된 입력입니다. 정수를 입력해주세요." << std::endl;
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			}
			else
			{
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				return choice;
			}
		}
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
