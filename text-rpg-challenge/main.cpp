#include <iostream>
#include "GameManager/UIManager.h"
#include "GameState/DungeonGameState.h"
#include "GameMode/DungeonGameMode.h"


int main()
{
    using namespace TextRPG;

    DungeonGameState dungeonGameState;
    DungeonGameMode dungeonGameMode;

    dungeonGameMode.Setup(&dungeonGameState, &UIManager::GetInstance());
    dungeonGameMode.Run();

    return 0;
}
