#include "Player.h"

namespace TextRPG
{
	void Player::SetInfo(const std::string& name, int stats[])
	{
		Character::SetName(name);
		Character::Initalize(FStatContainer(stats[0], stats[1], stats[2], stats[3]));
	}

	void Player::TakeDamage(float damage)
	{
		// TODO: 데미지 계산 로직 구현
	}

	void Player::Attack(float damage)
	{
		// TODO: 공격 로직 구현
	}
}
