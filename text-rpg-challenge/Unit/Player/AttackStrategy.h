#pragma once
#include "../Character.h"
#include "../../Data/BattleData.h"
#include <iostream>
#include <algorithm>
#include <vector>

namespace TextRPG
{

	// 공격 전략 인터페이스
	class IAttackStrategy
	{
	public:
		virtual ~IAttackStrategy() = default;
		virtual AttackResult ExecuteAttack(Character* attacker, Character& target) = 0;

	protected:
		AttackResult _handleAttack(Character* attacker, std::string attackName, Character& target, std::vector<int> damages)
		{
			AttackResult result;
			result.AttackerName = attacker->GetName();
			result.AttackName = attackName;
			result.Damages = damages;
			result.TargetName = target.GetName();
			result.TargetHP = target.GetCurrentHP();
			return result;
		}
	};

	class NoviceAttackStrategy : public IAttackStrategy
	{
	public:
		AttackResult ExecuteAttack(Character* attacker, Character& target) override
		{
			std::vector<int> damages;
			int damage = std::max(1, attacker->GetPAtk() - target.GetPDef());
			damages.push_back(damage);
			target.TakeDamage(damage);
			return _handleAttack(attacker, "Punch!", target, damages);
		}
	};

	class WarriorAttackStrategy : public IAttackStrategy
	{
	public:
		AttackResult ExecuteAttack(Character* attacker, Character& target) override
		{
			std::vector<int> damages;
			int damage = std::max(1, static_cast<int>(attacker->GetPAtk() * 1.5) - target.GetPDef());
			damages.push_back(damage);
			target.TakeDamage(damage);
			return _handleAttack(attacker, "Swings sword!", target, damages);
		}
	};

	class RogueAttackStrategy : public IAttackStrategy
	{
	public:
		AttackResult ExecuteAttack(Character* attacker, Character& target) override
		{
			std::vector<int> damages;
			int damagePerHit = std::max(1, (attacker->GetPAtk() / 2) - target.GetPDef());
			int hits = 5;
			int totalDamage = damagePerHit * hits;
			damages.assign(hits, damagePerHit);
			target.TakeDamage(totalDamage);
			return _handleAttack(attacker, "Stabs dagger!", target, damages);
		}
	};

	class MageAttackStrategy : public IAttackStrategy
	{
	public:
		AttackResult ExecuteAttack(Character* attacker, Character& target) override
		{
			std::vector<int> damages;
			int damage = std::max(1, attacker->GetMAtk() - target.GetMDef());
			damages.push_back(damage);
			target.TakeDamage(damage);
			return _handleAttack(attacker, "Fires fireball!", target, damages);
		}
	};

	class ArcherAttackStrategy : public IAttackStrategy
	{
	public:
		AttackResult ExecuteAttack(Character* attacker, Character& target) override
		{
			std::vector<int> damages;
			int damagePerHit = std::max(1, static_cast<int>(attacker->GetPAtk() * 0.8) - target.GetPDef());
			int hits = 3;
			int totalDamage = damagePerHit * hits;
			damages.assign(hits, damagePerHit);
			target.TakeDamage(totalDamage);
			return _handleAttack(attacker, "Shoots arrow!", target, damages);
		}
	};
}