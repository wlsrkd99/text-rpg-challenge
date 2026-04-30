#include "UIManager.h"
#include <iostream>
#include <limits>
#include "../Unit/Player/Player.h"

namespace TextRPG
{
	namespace Manager
	{
		void UIManager::PrintTitle(const std::string& title)
		{
			std::cout << "===========================================" << std::endl;
			std::cout << title << std::endl;
			std::cout << "===========================================" << std::endl;
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

		void UIManager::PrintPlayerStats(const Unit::Player& player)
		{
			std::cout << "====================================" << std::endl;
			std::cout << player.GetName() << "'s Stats" << std::endl;
			std::cout << "====================================" << std::endl;
			std::cout << "HP: " << player.GetHP() << "    MP: " << player.GetMP() << std::endl;
			std::cout << "Attack: " << player.GetAttack() << "    Defense: " << player.GetDefense() << std::endl;
			std::cout << "====================================" << std::endl;
		}
	}
}
