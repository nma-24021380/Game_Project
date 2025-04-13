#include "SceneManager.h"
#include "Card.h"
#include "Game.h"
#include "Defs.h"
#include "Utils.h"

SceneState currentScene;

SceneState getScene()
{
    return currentScene;
}

void changeScene(SceneState scene)
{
    currentScene = scene;
}

void changeMap(TileSet* tileMap, int &playerX, int &playerY, int direction)
{
    switch(direction)
    {
        case 0:
            playerY = SCREEN_HEIGHT - PLAYER_HEIGHT - 1;
            if (tileMap->getMapY() > 0)
            {
                tileMap->changeTileMap(tileMap->getMapX(), tileMap->getMapY() - 1);
            }
            break;
        case 1:
            playerY = 1;
            tileMap->changeTileMap(tileMap->getMapX(), tileMap->getMapY() + 1);
            break;
        case 2:
            playerX = SCREEN_WIDTH - PLAYER_WIDTH - 1;
            if (tileMap->getMapX() > 0)
            {
                tileMap->changeTileMap(tileMap->getMapX() - 1, tileMap->getMapY());
            }
            break;
        case 3:
            playerX = 1;
            tileMap->changeTileMap(tileMap->getMapX() + 1, tileMap->getMapY());
            break;
    }
}

void renderAdventure(TileSet* tileMap)
{
    if (tileMap != nullptr) tileMap->render();
}
