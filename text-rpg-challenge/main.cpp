#include <iostream>
#include "GameManager/UIManager.h"
#include "GameState/DungeonGameState.h"
#include "GameMode/DungeonGameMode.h"


int main()
{
    TextRPG::Manager::UIManager uiManager;
    TextRPG::GameState::DungeonGameState dungeonGameState;
    TextRPG::GameMode::DungeonGameMode dungeonGameMode;

    dungeonGameMode.Setup(&dungeonGameState, &uiManager);
    dungeonGameMode.Run();

    return 0;
}
