#pragma once
#include <stack>

namespace TextRPG
{
	/**
	 * @brief 제네릭 상태 머신 클래스입니다. 상태 스택을 관리하여 이전 상태로의 복귀를 지원합니다.
	 * @tparam T 상태를 나타내는 열거형(Enum) 타입
	 */
	template <typename T>
	class StateMachine
	{
	private:
		std::stack<T> m_StateStack;

	public:
		/** @brief 현재 상태 위에 새로운 상태를 쌓습니다. (예: 인벤토리 팝업) */
		void PushState(T state)
		{
			m_StateStack.push(state);
		}

		/** @brief 현재 상태를 제거하고 이전 상태로 돌아갑니다. (예: 창 닫기) */
		void PopState()
		{
			if (!m_StateStack.empty())
			{
				m_StateStack.pop();
			}
		}

		/** @brief 기존 상태를 모두 지우고 새로운 상태로 완전히 교체합니다. (예: 마을 -> 던전 진입) */
		void ChangeState(T state)
		{
			while (!m_StateStack.empty())
			{
				m_StateStack.pop();
			}
			m_StateStack.push(state);
		}

		/** @brief 현재(가장 위) 상태를 반환합니다. */
		T GetCurrentState() const
		{
			if (m_StateStack.empty()) return static_cast<T>(0);
			return m_StateStack.top();
		}
	};
}