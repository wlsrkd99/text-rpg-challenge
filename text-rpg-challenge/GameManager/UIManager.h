#pragma once

#include <string>
#include <vector> // std::vector를 사용하기 위해 추가
#include <iostream>
#include "../Unit/Player/Player.h"

namespace TextRPG
{
	class UIManager
	{

	public:
		UIManager() {};

		/** @brief 게임 타이틀을 형식에 맞게 출력합니다. */
		void PrintTitle(const std::string& title);

		/** @brief 일반 메시지를 출력합니다. */
		void PrintMessage(const std::string& message);

		/** @brief 프롬프트를 띄우고 사용자로부터 문자열을 입력받습니다. */
		std::string GetStringInput(const std::string& prompt);

		/** @brief 프롬프트를 띄우고 사용자로부터 특정 타입의 값들을 입력받습니다. */
		template<typename... Args>
		void GetInputs(const std::string& prompt, Args&... args)
		{
			while (true)
			{
				std::cout << prompt;
				using expander = int[];
				(void)expander{ 0, ((void)(std::cin >> args), 0)... };

				if (std::cin.fail())
				{
					std::cout << "Invalid input. Please enter valid values." << std::endl;
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

		/** @brief 플레이어의 현재 스탯을 형식에 맞게 출력합니다. */
		void PrintPlayerStats(const Player& player);

		/** @brief 기본 아이템을 출력합니다. */
		void PrintDefaultItem(const std::vector<std::pair<std::string, int>>& receivedItems);

		/** @brief 캐릭터 강화 메뉴를 출력합니다. */
		void PrintUpgradeMenu(const Player& player);

		/** @brief 스탯 효과를 출력합니다. */
		void PrintStatEffects();
		
		/** @brief 스탯 분배 UI를 출력합니다. */
		void PrintStatDistributionUI(const std::vector<std::string>& statNames, const std::array<int, static_cast<int>(EStatType::ST_Count)>& baseStats, int remainingPoints);
	};
}
