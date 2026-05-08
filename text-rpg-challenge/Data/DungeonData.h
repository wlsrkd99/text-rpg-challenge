#pragma once
#include <vector>

namespace TextRPG
{
	class Monster;

	/**
	 * @brief 던전의 개별 방을 나타내는 데이터 구조체입니다.
	 */
	struct Room
	{
		int RoomID = 0;
		Monster* RoomMonster = nullptr; // 방에 배치된 몬스터
		bool bIsCleared = false;        // 클리어 여부
		bool bIsBossRoom = false;       // 보스방 여부
	};

	/**
	 * @brief 던전의 한 층(Floor)을 나타내는 클래스입니다. 방 목록을 관리합니다.
	 */
	class DungeonFloor
	{
	private:
		int m_FloorLevel = 1;
		std::vector<Room> m_Rooms;

	public:
		DungeonFloor() = default;
		~DungeonFloor()
		{
			ClearRooms();
		}
		/** @brief 할당된 메모리를 해제하고 방 목록을 초기화합니다. */
		void ClearRooms();

		/** @brief 새로운 층의 방들과 몬스터를 무작위로 생성합니다. */
		void GenerateRooms(int floorLevel);

		Monster* _handleRandomMonster(bool bIsBoss);

		/** @brief 일반 방을 모두 클리어하여 보스방이 열렸는지 확인합니다. */
		bool IsBossUnlocked() const;


		std::vector<Room>& GetRooms() { return m_Rooms; }
		const std::vector<Room>& GetRooms() const { return m_Rooms; }
		int GetFloorLevel() const { return m_FloorLevel; }

		Room* GetRoom(int roomID);
	};
}
