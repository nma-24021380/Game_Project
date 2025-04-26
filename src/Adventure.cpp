#include "Adventure.h"
#include "SceneManager.h"
#include "Player.h"
#include "Enemy.h"
#include "TileSet.h"
#include "Utils.h"
#include "Game.h"
#include "Defs.h"
#include <iostream>

SDL_Texture* adv_guide = nullptr;
bool adv_guide_on = false;

void startAdventure()
{
    if (!adv_guide) adv_guide = loadTexture("assets/adv_guide.png");

    if (!player) player = new Player("Player", 100, 5, PLAYER_INIT_X, PLAYER_INIT_Y);
    player->changeState(State::IDLE);

    if (!tile_map) tile_map = new TileSet("assets/tileset.png");

    if (Mix_PlayingMusic()) Mix_HaltMusic();
    Mix_FadeOutMusic(1000);
    Mix_FadeInMusic(adv_bgm, -1, 1000);
}

void cleanAdventure()
{
    if (adv_guide)
    {
        SDL_DestroyTexture(adv_guide);
        adv_guide = nullptr;
    }

    if (tile_map)
    {
        delete tile_map;
        tile_map = nullptr;
    }

    if (player)
    {
        delete player;
        player = nullptr;
    }
}

void restartAdventure()
{
    player->restart();
    Enemy::restart();

    tile_map->changeTileMap(0, 0);

    startAdventure();
}

void handleAdventure()
{
    if (Game::event.type == SDL_KEYDOWN && Game::event.key.keysym.sym == SDLK_g)
    {
        adv_guide_on = !adv_guide_on;

        if (click_sfx) Mix_PlayChannel(-1, click_sfx, 0);
    }
}

bool checkWinning()
{
    if (tile_map->getMapX() == 1 && tile_map->getMapY() == 0)
    {
        SDL_Rect rect = player->getPos();
        int edges[4][2] =
        {
            {rect.x + rect.w / 2, rect.y + rect.h / 2}, // top
            {rect.x + rect.w / 2, rect.y + rect.h},     // bottom
            {rect.x, rect.y + rect.h / 2},              // left
            {rect.x + rect.w, rect.y + rect.h / 2}      // right
        };

        for (int i = 0; i < 4; ++i)
        {
            int tileID = tile_map->getTileID(edges[i][0] / TILE_SIZE, edges[i][1] / TILE_SIZE);
            if (tileID == 3)
            {
                return true;
            }
        }
    }
    return false;
}

void renderAdventure()
{
    if (getScene() != SceneState::ADVENTURE) return;

    if (tile_map) tile_map->render();   // render map

    for (auto enemy : enemies)  // enemy dung sau
    {
        if (enemy && enemy->getPos().y < player->getPos().y) enemy->render();
    }

    if (player) player->render();

    for (auto enemy : enemies)  // enemy dung truoc
    {
        if (enemy && enemy->getPos().y >= player->getPos().y) enemy->render();
    }

    SDL_Color color = {0, 0, 0, 255};   // render guide
    renderText("G", color, 50, 15, 15);

    if (adv_guide && adv_guide_on)
    {
        SDL_Rect destRect = {(SCREEN_WIDTH - 824) / 2, (SCREEN_HEIGHT - 675) / 2, 824, 675};
        SDL_RenderCopy(Game::renderer, adv_guide, NULL, &destRect);
    }
}

