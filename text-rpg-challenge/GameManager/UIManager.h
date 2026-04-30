#pragma once

#include <string>

namespace TextRPG
{
	namespace Unit { class Player; }
	//namespace Unit { class Monster; }

	namespace Manager
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

			/** @brief 프롬프트를 띄우고 사용자로부터 정수 2개를 입력받습니다. */
			void GetTwoIntsInput(const std::string& prompt, int& val1, int& val2);

			/** @brief 플레이어의 현재 스탯을 형식에 맞게 출력합니다. */
			void PrintPlayerStats(const Unit::Player& player);

		};
	}
}
