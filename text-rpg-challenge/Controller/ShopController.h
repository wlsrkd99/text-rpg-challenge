#pragma once
#include "../GameManager/UIManager.h"
#include "../Shop/PotionShop.h"
#include "../Core/User.h"
#include "../Core/StateMachine.h"
#include "../Enums/EGameState.h"

namespace TextRPG
{
	enum class EShopState
	{
		Main,
		Buy,
		Sell,
		Craft
	};

	class ShopController
	{
	private:
		UIManager* m_UI = nullptr;
		PotionShop* m_PotionShop = nullptr;
		User* m_User = nullptr;
		StateMachine<EGameState>* m_GlobalStateMachine = nullptr;
		StateMachine<EShopState> m_LocalStateMachine;

	public:
		ShopController() = default;

		/**
		 * @brief ShopController를 초기화하고 필요한 의존성을 주입합니다.
		 * @param ui UIManager 객체 포인터
		 * @param potionShop PotionShop 객체 포인터
		 * @param user User 객체 포인터
		 * @param globalSM 전역 상태 머신 포인터
		 */
		void Setup(UIManager* ui, PotionShop* potionShop, User* user, StateMachine<EGameState>* globalSM);

		/** @brief 상점 컨트롤러의 메인 처리 로직을 실행합니다. */
		void Process();

	private:
		void _handleShopBuyLogic(int choice, int count);
		void _handleShopSellLogic(int choice, int count);
		void _handleShopCraftLogic(int choice);
	};
}