#include "Utils.h"
#include "Defs.h"
#include "SceneManager.h"
#include "Game.h"
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <iostream>

SDL_Texture* loadTexture(const char* file)
{
    SDL_Texture* texture = IMG_LoadTexture(Game::renderer, file);
    if (texture == nullptr)
    {
        std::cout << "Failed to load texture! Error: " << IMG_GetError() << std::endl;
    }
    return texture;
}

bool checkEnemy(const SDL_Rect &a, const SDL_Rect &b)
{
    return SDL_HasIntersection(&a, &b);
}

bool checkCollision(TileSet* tileMap, int x, int y)
{
    int mapX = (x + PLAYER_WIDTH / 2) / TILE_SIZE;
    int mapY = (y + PLAYER_HEIGHT) / TILE_SIZE;

    if (mapX < 0 || mapX >= MAP_WIDTH || mapY < 0 || mapY >= MAP_HEIGHT) return true;

    int tileID = tileMap->getTileID(mapX, mapY);
    return (tileID == 1 || tileID == 2);
}

bool atBorder(int x, int y)
{
    return (x <= 0 || x >= SCREEN_WIDTH - PLAYER_WIDTH ||
            y <= 0 || y >= SCREEN_HEIGHT - PLAYER_HEIGHT);
}

void renderText(const std::string text, SDL_Color color, int size, int x, int y)
{
    TTF_Font* font = TTF_OpenFont("assets/font.ttf", size);
    if (font == nullptr) std::cout << "Failed to load font! Error " << TTF_GetError();

    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(Game::renderer, surface);
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
}
