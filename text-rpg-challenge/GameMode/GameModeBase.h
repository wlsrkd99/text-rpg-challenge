#pragma once
#include "../GameState/GameStateBase.h"
#include "../GameManager/UIManager.h"

namespace TextRPG
{
	namespace GameMode
	{
		class GameModeBase
		{
		protected:
			GameState::GameStateBase* m_State = nullptr;
			Manager::UIManager* m_UI = nullptr;

		public:
			GameModeBase() = default;

			/* @brief 게임을 초기화합니다.*/
			virtual void InitGame() = 0;
			
			/* @brief 게임 루프를 실행합니다.*/
			virtual void Run() = 0;

			/**
			 * @brief 게임 모드의 UI 및 게임 상태 종속성을 설정합니다.
			 * @param state 게임 상태 객체 포인터
			 * @param ui UI 관리자 객체 포인터
			 */
			void Setup(GameState::GameStateBase* state, Manager::UIManager* ui)
			{
				m_State = state;
				m_UI = ui;
			}
		};
	}
}
