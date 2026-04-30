#pragma once

#include <string>

namespace TextRPG
{
	namespace Unit
	{
		class Character
		{
		protected:
			std::string m_Name;
			int m_MaxHp;
			int m_Hp;
			int m_MaxMp;
			int m_Mp;
			int m_Atk;
			int m_Def;

		public:
			Character() {};

			const std::string& GetName() const;
			int GetMaxHP() const;
			int GetMaxMP() const;
			int GetHP() const;
			int GetMP() const;
			int GetAttack() const;
			int GetDefense() const;


			virtual void TakeDamage(float damage) = 0;
			virtual void Attack(float damage) = 0;

		};
	}
}
