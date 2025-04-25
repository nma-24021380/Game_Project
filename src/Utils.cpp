#include "Utils.h"
#include "Defs.h"
#include "SceneManager.h"
#include "Game.h"
#include "Enemy.h"
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <iostream>

Mix_Chunk* explosion_sfx = nullptr;
Mix_Chunk* heal_sfx = nullptr;
Mix_Chunk* click_sfx = nullptr;
Mix_Music* adv_bgm = nullptr;
Mix_Music* battle_bgm = nullptr;

void loadMusic()
{
    if (!explosion_sfx) explosion_sfx = Mix_LoadWAV("assets/sounds/explosion.wav");

    if (!heal_sfx) heal_sfx = Mix_LoadWAV("assets/sounds/heal.wav");

    if (!click_sfx) click_sfx = Mix_LoadWAV("assets/sounds/click.wav");

    if (!adv_bgm) adv_bgm = Mix_LoadMUS("assets/sounds/adv_bgm.mp3");

    if (!battle_bgm) battle_bgm = Mix_LoadMUS("assets/sounds/battle_bgm.mp3");
}

void cleanMusic()
{
    if (explosion_sfx)
    {
        Mix_FreeChunk(explosion_sfx);
        explosion_sfx = nullptr;
    }

    if (heal_sfx)
    {
        Mix_FreeChunk(heal_sfx);
        heal_sfx = nullptr;
    }

    if (click_sfx)
    {
        Mix_FreeChunk(click_sfx);
        click_sfx = nullptr;
    }

    if (adv_bgm)
    {
        Mix_FreeMusic(adv_bgm);
        adv_bgm = nullptr;
    }

    if (battle_bgm)
    {
        Mix_FreeMusic(battle_bgm);
        battle_bgm = nullptr;
    }
}

SDL_Texture* loadTexture(const char* file)
{
    SDL_Texture* texture = IMG_LoadTexture(Game::renderer, file);

    if (texture == nullptr)
    {
        std::cout << "Failed to load texture! Error: " << IMG_GetError() << std::endl;
        return nullptr;
    }

    return texture;
}

bool checkEntity(const SDL_Rect &a, const SDL_Rect &b)
{
    SDL_Rect a_pos = {a.x, a.y + a.h / 2, a.w, a.h / 2};
    SDL_Rect b_pos = {b.x, b.y + b.h / 2, b.w, b.h / 2};

    if (SDL_HasIntersection(&a_pos, &b_pos)) return true;

    return false;
}

bool checkCollision(int w, int h, int x, int y)
{
    int edges[4][2] =
    {
        {x + w / 2, y + h / 2}, // top
        {x + w / 2, y + h},     // bottom
        {x, y + h / 2},         // left
        {x + w, y + h / 2}      // right
    };

    for (int i = 0; i < 4; ++i)
    {
        int map_x = edges[i][0] / TILE_SIZE;
        int map_y = edges[i][1] / TILE_SIZE;

        if (map_x < 0 || map_x >= MAP_WIDTH || map_y < 0 || map_y >= MAP_HEIGHT) return true;

        int tileID = tile_map->getTileID(map_x, map_y);
        if (tileID == 1 || tileID == 2) return true;
    }
    return false;
}

bool atBorder(int x, int y)
{
    return (x <= 0 || x >= SCREEN_WIDTH - PLAYER_WIDTH ||
            y <= 0 || y >= SCREEN_HEIGHT - PLAYER_HEIGHT);
}

bool availablePosition(int x, int y)
{
    SDL_Rect position = {x, y, ENEMY_WIDTH, ENEMY_HEIGHT};
    SDL_Rect entity_pos = player->getPos();

    if (checkCollision(position.w, position.h, x, y)
        || atBorder(x, y))
    {
        return false;
    }

    if (SDL_HasIntersection(&entity_pos, &position)) return false;

    for (auto enemy : enemies)
    {
        entity_pos = enemy->getPos();
        if (SDL_HasIntersection(&entity_pos, &position)) return false;
    }
    return true;
}

void renderText(const std::string text, SDL_Color color, int size, int x, int y)
{
    TTF_Font* font = TTF_OpenFont("assets/font.ttf", size);
    if (font == nullptr)
    {
        std::cout << "Failed to load font! Error " << TTF_GetError();
        return;
    }
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
    if (surface == nullptr)
    {
        std::cout << "Failed to create text surface! Error " << SDL_GetError();
        TTF_CloseFont(font);
        return;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(Game::renderer, surface);
    if (surface == nullptr)
    {
        std::cout << "Failed to create text texture from surface! Error " << SDL_GetError();
        SDL_FreeSurface(surface);
        TTF_CloseFont(font);
        return;
    }

    SDL_Rect destRect = {x, y, surface->w, surface->h};
    SDL_RenderCopy(Game::renderer, texture, nullptr, &destRect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
    TTF_CloseFont(font);
}

void renderHP(Entity* entity, int x, int y)
{
    int w = 100, h = 10;

    SDL_Rect bar = {x, y, w, h};
    SDL_Rect hpBar = {x, y, w * entity->health / entity->max_health, h};
    SDL_Texture* outlineBar = loadTexture("assets/hp_bar.png");

    SDL_SetRenderDrawColor(Game::renderer, 100, 100, 100, 255); // nen mau xam
    SDL_RenderFillRect(Game::renderer, &bar);

    SDL_SetRenderDrawColor(Game::renderer, 255, 0, 0, 255); // thanh hp mau do
    SDL_RenderFillRect(Game::renderer, &hpBar);

    SDL_RenderCopy(Game::renderer, outlineBar, nullptr, &bar); // duong vien

    SDL_DestroyTexture(outlineBar);
}
