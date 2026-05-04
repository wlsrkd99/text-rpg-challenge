#pragma once

namespace TextRPG
{
	enum class EGameState
	{
		GS_NONE,             // 초기 상태 (초기화 및 캐릭터 생성 로직 연결)
		GS_TOWN,             // 마을 (던전 진입, 상점, 휴식, 인벤토리 등 분기점)
		GS_SHOP,             // 상점 (아이템 구매/판매)
		GS_DUNGEON_EXPLORE,  // 던전 탐험 중 (이동, 이벤트 발생 대기)
		GS_BATTLE,           // 전투 중 (턴제 전투 진행)
		GS_INVENTORY,        // 인벤토리 (아이템 확인 및 사용)
		GS_STAT_UPGRADE,     // 스탯 강화 창
		GS_GAMEOVER,         // 플레이어 사망 또는 게임 클리어
	};
}
