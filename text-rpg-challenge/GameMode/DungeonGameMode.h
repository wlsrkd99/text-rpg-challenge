#pragma once
#include "GameModeBase.h"
#include "../Core/StateMachine.h"
#include "../Controller/BattleController.h"
#include "../Controller/DungeonController.h"
#include "../Controller/InventoryController.h"
#include "../Controller/JobController.h"
#include "../Controller/StatController.h"
#include "../Controller/ShopController.h"
#include "../Controller/TownController.h"
#include "../Data/DungeonData.h"
#include "../Enums/EGameState.h"
#include "../GameState/DungeonGameState.h"
#include "../GameManager/UIManager.h"
#include "../Shop/PotionShop.h"

#include <functional>

namespace TextRPG
{
	class Monster;

	class DungeonGameMode : public GameModeBase
	{
	public:
		DungeonGameMode() = default;

		virtual void Setup(GameStateBase* state, UIManager* ui) override;

		void ProcessInventory();
		void ProcessPotionShop();

		virtual void Run() override;
		virtual void InitGame() override;

	private:
		bool m_bGameOver = false; // 게임 종료 여부
		bool m_bItemUsedSuccessfully = false; // 아이템 사용 성공 여부
		bool m_bExitStatUpgrade = false; // 스탯 강화 루프 탈출 조건

		PotionShop m_PotionShop;

		BattleController m_BattleController;
		DungeonController m_DungeonController;
		InventoryController m_InventoryController;
		JobController m_JobController;
		ShopController m_ShopController;
		StatController m_StatController;
		TownController m_TownController;

		StateMachine<EGameState> m_StateMachine;

		DungeonGameState* GetDungeonState() const { return static_cast<DungeonGameState*>(m_State); }
		Monster* CurrentMonster() {
			Room* room = GetDungeonState() ? GetDungeonState()->GetCurrentRoom() : nullptr;
			return room ? room->RoomMonster : nullptr; 
		}
		/** @brief 기본 스탯 분배 과정을 처리합니다. */
		void _handleBaseStatDistribution();
		
		/**
		 * @brief 캐릭터에게 기본 아이템을 지급하고, 지급된 아이템 목록을 반환합니다.
		 * @return 지급된 아이템의 이름과 개수 목록
		 */
		std::vector<std::pair<std::string, int>> _handleReceiveDefaultItem();
	};
}