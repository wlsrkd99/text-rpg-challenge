#pragma once

#include "../Character.h"
#include <string>

namespace TextRPG
{
	namespace Unit
	{
		class Player : public Character
		{
		private:
			int m_Level = 0;
			long m_Exp = 0;


		public:
			Player() {};

			virtual void TakeDamage(float damage);
			virtual void Attack(float damage);

			/**
			 * @brief 플레이어의 이름과 모든 스탯을 설정합니다.
			 * @param name 플레이어 이름
			 * @param hp 체력
			 * @param mp 마력
			 * @param atk 공격력
			 * @param def 방어력
			 */
			void SetInfo(const std::string& name, int hp, int mp, int atk, int def);
		};
	}
}
