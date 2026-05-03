#include <iostream>
#include "GameManager/UIManager.h"
#include "GameState/DungeonGameState.h"
#include "GameMode/DungeonGameMode.h"


int main()
{
    using namespace TextRPG;

    UIManager uiManager;
    DungeonGameState dungeonGameState;
    DungeonGameMode dungeonGameMode;

    dungeonGameMode.Setup(&dungeonGameState, &uiManager);
    dungeonGameMode.Run();

    return 0;
}
