#include "SceneManager.h"
#include "Card.h"
#include "Game.h"
#include "Defs.h"
#include "Utils.h"
#include <SDL_image.h>
#include <iostream>

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

void renderBattle(Entity* player, Entity* enemy)
{
    SDL_Texture* background = IMG_LoadTexture(Game::renderer, "assets/battle_bg.png");
    SDL_RenderCopy(Game::renderer, background, nullptr, nullptr);

    SDL_Color color = {0, 0, 0, 255};

    renderHP(player, 250, SCREEN_HEIGHT / 2 - 20);
    renderText(player->name, color, 16, 250, SCREEN_HEIGHT / 2 - 40);

    renderHP(enemy, SCREEN_WIDTH - 250, SCREEN_HEIGHT / 2 - 20);
    renderText(enemy->name, color, 16, SCREEN_WIDTH - 250, SCREEN_HEIGHT / 2 - 40);

    SDL_DestroyTexture(background);
}

void renderHP(Entity* entity, int x, int y)
{
    int w = 100, h = 10;

    SDL_Rect bar = {x, y, w, h};
    SDL_Rect hpBar = {x, y, w * entity->health / entity->max_health, h};
    SDL_Texture* outlineBar = IMG_LoadTexture(Game::renderer, "assets/hp_bar.png");

    SDL_SetRenderDrawColor(Game::renderer, 100, 100, 100, 255); // nen mau xam
    SDL_RenderFillRect(Game::renderer, &bar);
    SDL_SetRenderDrawColor(Game::renderer, 255, 0, 0, 255); // thanh hp mau do
    SDL_RenderFillRect(Game::renderer, &hpBar);
    SDL_RenderCopy(Game::renderer, outlineBar, nullptr, &bar); // duong vien
}
