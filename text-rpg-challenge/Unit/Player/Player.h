#pragma once

#include "../Character.h"
#include <string>

namespace TextRPG
{
	class Player : public Character
	{
	private:
		int m_Level = 0;
		long m_Exp = 0;

		int m_HPPotionCount = 5; // 초기 포션 개수
		int m_MPPotionCount = 5; // 초기 포션 개수

	public:
		Player(const std::string& name, const FStatContainer& stats)
			: Character(name, stats) { }

		virtual void TakeDamage(float damage);
		virtual void Attack(float damage);

		/**
		 * @brief 플레이어의 이름과 모든 스탯을 설정합니다.
		 * @param name 플레이어 이름
		 * @param stats 스탯 배열
		 */
		void SetInfo(const std::string& name, int stats[]);
	};
}
