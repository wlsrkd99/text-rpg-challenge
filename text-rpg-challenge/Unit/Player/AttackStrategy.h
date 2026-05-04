#pragma once
#include "../Character.h"
#include <iostream>
#include <algorithm>

namespace TextRPG
{
	// 공격 전략 인터페이스
	class IAttackStrategy
	{
	public:
		virtual ~IAttackStrategy() = default;
		virtual void ExecuteAttack(Character* attacker, Character& target) = 0;
	};

	class NoviceAttackStrategy : public IAttackStrategy
	{
	public:
		void ExecuteAttack(Character* attacker, Character& target) override
		{
			int damage = std::max(1, attacker->GetPAtk() - target.GetPDef());
			std::cout << "[Novice] Punches! -> " << damage << " damage to " << target.GetName() << "!" << std::endl;
			target.TakeDamage(damage);
		}
	};

	class WarriorAttackStrategy : public IAttackStrategy
	{
	public:
		void ExecuteAttack(Character* attacker, Character& target) override
		{
			int damage = std::max(1, static_cast<int>(attacker->GetPAtk() * 1.5) - target.GetPDef());
			std::cout << "[Warrior] Swings sword! -> " << damage << " damage to " << target.GetName() << "!" << std::endl;
			target.TakeDamage(damage);
		}
	};

	class RogueAttackStrategy : public IAttackStrategy
	{
	public:
		void ExecuteAttack(Character* attacker, Character& target) override
		{
			int hits = 5;
			int damagePerHit = std::max(1, (attacker->GetPAtk() / 2) - target.GetPDef());
			int totalDamage = damagePerHit * hits;
			std::cout << "[Rogue] Stabs dagger! -> " << damagePerHit << " damage to " << target.GetName() 
					  << "! (x" << hits << ")" << std::endl;
			target.TakeDamage(totalDamage);
		}
	};

	class MageAttackStrategy : public IAttackStrategy
	{
	public:
		void ExecuteAttack(Character* attacker, Character& target) override
		{
			int damage = std::max(1, attacker->GetMAtk() - target.GetMDef());
			std::cout << "[Mage] Fires fireball! -> " << damage << " damage to " << target.GetName() << "!" << std::endl;
			target.TakeDamage(damage);
		}
	};

	class ArcherAttackStrategy : public IAttackStrategy
	{
	public:
		void ExecuteAttack(Character* attacker, Character& target) override
		{
			int hits = 3;
			int damagePerHit = std::max(1, static_cast<int>(attacker->GetPAtk() * 0.8) - target.GetPDef());
			int totalDamage = damagePerHit * hits;
			std::cout << "[Archer] Shoots arrow! -> " << damagePerHit << " damage to " << target.GetName() 
					  << "! (x" << hits << ")" << std::endl;
			target.TakeDamage(totalDamage);
		}
	};
}