#include "SceneManager.h"
#include "TileSet.h"
#include "Card.h"
#include "Game.h"
#include "Defs.h"


//SceneType SceneManager::currentScene = SceneType::ADVENTURE;
SceneType currentScene = SceneType::ADVENTURE;


SceneType getScene()
{
    return currentScene;
}

void setScene(SceneType scene)
{
    currentScene = scene;
}

void renderAdventure()
{
    TileSet tileSet(Game::renderer, "assets/grass.png", TILE_SIZE);
    tileSet.render(Game::renderer, grassMapData);

}

void renderBattle()
{
    SDL_SetRenderDrawBlendMode(Game::renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(Game::renderer, 0, 0, 0, 50);
    SDL_RenderFillRect(Game::renderer, NULL);
    // tạm để màn hình đen
}
