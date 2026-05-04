#pragma once

#include "../Unit/Player/Player.h"
#include "../Enums/EJobType.h"
#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <functional>

namespace TextRPG
{
	class UIManager
	{

	public:
		UIManager() {};

#pragma region Basic I/O
	public:
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
#pragma endregion

#pragma region Game Data Display
	public:
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
#pragma endregion

#pragma region User Interaction Flow Control
	private:
		/** @brief Health와 Mana 스탯 분배 UI 상호작용을 처리합니다. */ // 내부 함수 네이밍 규칙 적용
		void _handleHealthManaDistributionUI(std::array<int, static_cast<int>(EStatType::ST_Count)>& outBaseStats);

		/** @brief 핵심 스탯 분배 UI 상호작용을 처리합니다. */ // 내부 함수 네이밍 규칙 적용
		void _handleCoreStatDistributionUI(std::array<int, static_cast<int>(EStatType::ST_Count)>& outBaseStats);
		
	public:
		/** @brief 직업 변경 메뉴를 표시하고 사용자의 선택을 받아 반환합니다. */
		EJobType ShowClassChangeMenuAndGetChoice(const Player& player);

		/** @brief 캐릭터 생성 시 초기 스탯 분배 과정을 UI를 통해 처리하고 결과를 반환합니다. */
		std::array<int, static_cast<int>(EStatType::ST_Count)> PerformInitialStatDistribution();

		/**
		 * @brief 핵심 스탯 분배 과정을 처리하는 UI 헬퍼 함수
		 * @param getPoints 사용 가능한 총 포인트를 반환하는 함수
		 * @param spendPoints 포인트를 사용하고 성공 여부를 반환하는 함수
		 * @param getStatsArray 현재 스탯 배열을 반환하는 함수
		 * @param applyStatChange 선택된 스탯에 포인트를 적용하는 함수
		 * @param onUpdate 스탯이 업데이트될 때마다 호출될 콜백 함수 (선택 사항)
		 */
		void ShowStatDistribution(
			std::function<int()> getPoints,
			std::function<bool(int)> spendPoints,
			std::function<const std::array<int, static_cast<int>(EStatType::ST_Count)>&()> getStatsArray,
			std::function<void(EStatType, int)> applyStatChange,
			std::function<void()> onUpdate = nullptr
		);
#pragma endregion
	};
}
