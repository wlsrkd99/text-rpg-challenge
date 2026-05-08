#pragma once
#include "../Core/Delegate.h"
#include "../Enums/EJobType.h"
#include "../Enums/EBattleActionType.h"
#include "../Unit/Player/Player.h"
#include "../Unit/Monster/Monster.h"
#include "../Item/Recipe.h"
#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <functional>
#include <utility>
#include "../Data/DungeonData.h"

namespace TextRPG
{
	struct MenuConfig
	{
		std::string Title;
		std::vector<std::string> Infos;   // 선택지 전 보여줄 추가 정보들
		std::vector<std::string> Options; // 실제 메뉴 선택지들
		std::string Prompt = "Choice: ";
	};

	struct AttackResult;
	class Inventory;

	class UIManager
	{

	public:
		UIManager() {};

		// Main Menu
		Delegate<int> OnMenuNavigationRequested;

		// Initialization
		Delegate<std::string> OnCharacterNameRequested;
		Delegate<int, int> OnInitialStatRequested;

		// Stat
		Delegate<EStatType, int> OnStatUpgradeRequested;
		Delegate<int> OnStatUpgradeMenuSelected;

		// Town
		Delegate<int> OnTownActionSelected;
		Delegate<EJobType> OnJobChangeRequested;
		Delegate<int> OnPotionShopActionSelected;
		Delegate<int, int> OnShopBuyRequested;
		Delegate<int, int> OnShopSellRequested;
		Delegate<int> OnShopCraftActionSelected;
		Delegate<std::string> OnRecipeSearchRequested;
		Delegate<int> OnRecipeCraftRequested;

		// Inventory & Item
		Delegate<int> OnInventoryActionSelected;
		Delegate<int> OnItemUseRequested;

		// Dungeon
		Delegate<int, int> OnDungeonRoomSelected;
		Delegate<EBattleActionType> OnBattleActionSelected;



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
				if(prompt.length() > 0)	std::cout << prompt;
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

		/** @brief 안내 메시지(제목과 설명)를 범용적인 양식으로 출력합니다. */
		void PrintGuide(const std::string& title, const std::string& description = "");

		/** @brief 범용적인 선택지 메뉴를 띄우고 사용자 입력을 받아 반환합니다. */
		int PromptMenu(const MenuConfig& config);

		/** @brief 캐릭터 이름을 묻고 입력을 검증한 후 이벤트를 발생시킵니다. */
		void PromptCharacterName();
#pragma endregion

	public:
		/** 
		 * @brief 메인메뉴를 선택하는 프롬프트를 띄우고 이벤트를 발생시킵니다.
		 * @param gold 현재 유저가 보유한 골드량
		 */
		void PromptMainMenuAction(int gold);


#pragma region Stat
	public:
		/** @brief 각 스탯이 어떤 능력치에 영향을 주는지 설명하는 정보를 출력합니다. */
		void DisplayStatEffects();

		/**
		 * @brief 현재 스탯 현황을 출력합니다.
		 * @param statNames 출력할 스탯 이름 목록 (Endurance, Strength, Agility, Intelligence, Wisdom)
		 * @param baseStats 캐릭터의 기본 스탯 배열
		 * @param remainingPoints 남은 스탯 포인트
		 */
		void DisplayStatAllocation(const std::vector<std::string>& statNames, const std::array<int, static_cast<int>(EStatType::ST_Count)>& baseStats, int remainingPoints);

		/**
		* @brief 캐릭터의 전체 스탯 정보를 출력합니다. (기본 스탯 + 파생 스탯)
		* @param player 출력할 플레이어 객체
		*/
		void DisplayCharacterSheet(const Player& player);

		/**
		 * @brief 업그레이드할 스탯과 포인트 양을 묻는 프롬프트입니다.
		 *		  입력 검증 후 이벤트만 발생시키며, 실제 로직 처리는 DungeonGameMode에서 구독하여 처리합니다.
		 * @param statNames 출력할 스탯 이름 목록 (Endurance, Strength, Agility, Intelligence, Wisdom)
		 * @param baseStats 캐릭터의 기본 스탯 배열
		 * @param remainingPoints 남은 스탯 포인트
		 */
		void PromptStatAllocation(const std::vector<std::string>& statNames, const std::array<int, static_cast<int>(EStatType::ST_Count)>& baseStats, int remainingPoints);

		void PromptStatUpgradeAction(const Player& player);

		/** @brief 초기 체력/마나 분배를 묻고 입력을 검증한 후 이벤트를 발생시킵니다. */
		void PromptInitialStatAllocation();

		/*
		 * @brief 기본 아이템을 출력합니다.
		 * @param items 출력할 기본 아이템 목록
		 */
		void DisplayDefaultItem(const std::vector<std::pair<std::string, int>>& items);

	private:
		/*
		 * @brief 플레이어의 기본 스탯을 출력하는 내부 함수입니다.
		 *		  DisplayCharacterSheet에서 호출되어 전체 캐릭터 시트를 구성합니다.
		 * @param player 출력할 플레이어 객체
		 * @note 기본 스탯: Endurance, Strength, Agility, Intelligence, Wisdom
		 */
		void _handleBaseStatSheet(const Player& player);

		/*
		 * @brief 플레이어의 파생 스탯을 출력하는 내부 함수입니다.
		 *		  DisplayCharacterSheet에서 호출되어 전체 캐릭터 시트를 구성합니다.
		 * @param player 출력할 플레이어 객체
		 * @note 파생 스탯: Health, Mana, Attack Power, Defense, Evasion Rate 등
		 */
		void _handleDerivedStatSheet(const Player& player);
#pragma endregion

#pragma region Dungeon
	public:


		/** @brief 던전 방 선택 프롬프트를 띄우고 이벤트를 발생시킵니다. */
		void PromptDungeonAction(const DungeonFloor& floor);

#pragma endregion

#pragma region Battle
	public:
		/** @brief 전투 행동을 선택하는 프롬프트를 띄우고 이벤트를 발생시킵니다. */
		void PromptBattleAction();

		/*
		 * @brief 공격 결과를 형식에 맞게 출력합니다. 
		 * @param result 출력할 공격 결과
		 */
		void DisplayAttackResult(const AttackResult& result);

		/*
		 * @brief 승리 결과를 출력합니다.
		 * @param data 출력할 승리 결과 데이터
		 */
		void DisplayBattleWinDetails(const BattleWinData& data);

		/*
		 * @brief 패배 결과를 출력합니다.
		 * @param data 출력할 패배 결과 데이터
		 */
		void DisplayBattleLoseDetails(const BattleLoseData& data);

#pragma endregion

#pragma region Inventory
	public:
		/*
		 * @brief 인벤토리 내용을 출력합니다.
		 * @param inventory 출력할 인벤토리 객체
		 */
		void DisplayInventory(const Inventory& inventory);

		/*
		 * @brief 아이템 사용 결과를 출력합니다.
		 * @param result 출력할 아이템 사용 결과
		 */
		void DisplayItemUseResult(const ItemUseResult& result);

		/*
		 * @brief 인벤토리에서 사용할 아이템을 선택하는 프롬프트입니다.
		 * @param inventory 출력할 인벤토리 객체
		 * @return 선택된 아이템의 ID, 취소 시 -1
		 */
		void PromptItemUse(const Inventory& inventory);

		void PromptInventoryAction(const Inventory& inventory);


#pragma endregion

#pragma region Town
	public:
		/**
		 * @brief 마을에서의 행동을 선택하는 프롬프트를 띄우고 이벤트를 발생시킵니다.
		 */
		void PromptTownAction();

		/**
		 * @brief 전직할 직업을 선택하는 프롬프트를 띄우고 이벤트를 발생시킵니다.
		 * @param player 출력 및 검증에 사용할 플레이어 객체
		 */
		void PromptJobChange(const Player& player);


		/**
		 * @brief 포션 상점에서의 행동을 선택하는 프롬프트를 띄우고 이벤트를 발생시킵니다.
		 */
		void PromptPotionShopAction();

		void PromptShopBuyAction(const Inventory& stock, int playerGold);

		void PromptShopSellAction(const Inventory& inventory);

		void PromptShopCraftAction();

		/**
		 * @brief 레시피 목록을 화면에 출력합니다.
		 * @param stock 재료 이름을 조회하기 위한 상점 인벤토리
		 */
		void DisplayRecipes(const std::vector<Recipe>& recipes, const Inventory& stock);

		/**
		 * @brief 상점 재고 목록과 유저 소지금 정보를 출력합니다.
		 * @param stock 상점 인벤토리 객체
		 * @param playerGold 플레이어 보유 골드
		 */
		void DisplayShopStock(const Inventory& stock, int playerGold);
#pragma endregion

	};
}
