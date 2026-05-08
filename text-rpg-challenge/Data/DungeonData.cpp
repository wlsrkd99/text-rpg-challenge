#include "DungeonData.h"
#include "../Unit/Monster/Monster.h"
#include "../Enums/EStatType.h"
#include <random>
#include <array>


namespace TextRPG
{
	void DungeonFloor::ClearRooms()
	{
		for(Room& room : m_Rooms)
		{
			if (room.RoomMonster)
			{
				delete room.RoomMonster;
				room.RoomMonster = nullptr;
			}
		}
		m_Rooms.clear();
	}

	void DungeonFloor::GenerateRooms(int floorLevel)
	{
		ClearRooms();
		m_FloorLevel = floorLevel;

		int numRooms = 3 + floorLevel;
		for (int i = 0; i < numRooms; i++)
		{
			Room newRoom;
			newRoom.RoomID = i + 1;
			newRoom.bIsCleared = false;
			newRoom.bIsBossRoom = (i == numRooms - 1);
			newRoom.RoomMonster = _handleRandomMonster(newRoom.bIsBossRoom);
			m_Rooms.push_back(newRoom);
		}
	}

	Monster* DungeonFloor::_handleRandomMonster(bool bIsBoss)
	{
		Monster* newMonster = new Monster("monster");
		std::array<int, static_cast<int>(EStatType::ST_Count)> stats;
		stats.fill(5);

		if (bIsBoss)
		{
			stats[static_cast<int>(EStatType::ST_Health)] = 200; // 체력
			stats[static_cast<int>(EStatType::ST_Strength)] = 60; // 공격력 영향
			stats[static_cast<int>(EStatType::ST_Endurance)] = 20; // 방어력 영향

			newMonster->Initialize("Dragon", stats);
			newMonster->SetDropExp(500);
			newMonster->SetDropGold(300);
			// 보스 전용 드랍 아이템 세팅 등...
			return newMonster;
		}

		// 일반 몬스터 랜덤 생성 (1 ~ 3)
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<int> dist(1, 3);

		int randType = 1;//dist(gen);
		switch (randType)
		{
		case 1:
			stats[static_cast<int>(EStatType::ST_Health)] = 30;
			stats[static_cast<int>(EStatType::ST_Strength)] = 15;
			newMonster->Initialize("Slime", stats);
			newMonster->SetDropExp(50);
			newMonster->SetDropGold(15);
			newMonster->SetDropItem(new ItemBase(1, "Slime Jelly", EItemType::IT_NONE, EItemGrade::IG_COMMON, 10, 1));
			break;
		case 2:
			stats[static_cast<int>(EStatType::ST_Health)] = 50;
			stats[static_cast<int>(EStatType::ST_Strength)] = 25;
			newMonster->Initialize("Goblin", stats);
			newMonster->SetDropExp(80);
			newMonster->SetDropGold(30);
			break;
		case 3:
			stats[static_cast<int>(EStatType::ST_Health)] = 80;
			stats[static_cast<int>(EStatType::ST_Strength)] = 35;
			newMonster->Initialize("Orc", stats);
			newMonster->SetDropExp(120);
			newMonster->SetDropGold(50);
			break;
		}

		return newMonster;
	}

	bool DungeonFloor::IsBossUnlocked() const
	{
		for (Room room : m_Rooms)
		{
			if (room.bIsCleared) continue;
			else if (!room.bIsBossRoom) return false;
		}
		return true;
	}
	Room* DungeonFloor::GetRoom(int roomID)
	{
		for (Room& room : m_Rooms)
		{
			if (room.RoomID == roomID)
			{
				return &room;
			}
		}
		return nullptr;
	}
}
