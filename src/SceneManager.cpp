#include "SceneManager.h"
#include "TileSet.h"
#include "Card.h"
#include "Game.h"
#include "Defs.h"
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>

SceneState currentScene;

SceneState getScene()
{
    return currentScene;
}

void setScene(SceneState scene)
{
    currentScene = scene;
}

void renderAdventure()
{
    TileSet tileSet(Game::renderer, "assets/grass.png", TILE_SIZE);
    tileSet.render(Game::renderer, grassMapData);

}

void renderBattle(Entity* player, Entity* enemy)
{
    SDL_Texture* background = IMG_LoadTexture(Game::renderer, "assets/background.png");
    SDL_RenderCopy(Game::renderer, background, nullptr, nullptr);

    renderHP(player);
    renderText(player->name, player->position.x, player->position.y - 40);
    renderHP(enemy);
    renderText(enemy->name, enemy->position.x, enemy->position.y - 40);

    SDL_DestroyTexture(background);
}

void renderHP(Entity* entity)
{
    int x = entity->position.x;
    int y = entity->position.y - 20;
    int w = 100, h = 10;

    SDL_Rect bar = {x, y, w, h};
    SDL_Rect hpBar = {x, y, w * entity->health / entity->maxHealth, h};
    SDL_Texture* outlineBar = IMG_LoadTexture(Game::renderer, "assets/hp_bar.png");

    SDL_SetRenderDrawColor(Game::renderer, 100, 100, 100, 255); // nen mau xam
    SDL_RenderFillRect(Game::renderer, &bar);
    SDL_SetRenderDrawColor(Game::renderer, 255, 0, 0, 255); // thanh hp mau do
    SDL_RenderFillRect(Game::renderer, &hpBar);
    SDL_RenderCopy(Game::renderer, outlineBar, nullptr, &bar); // duong vien
}

void renderText(const std::string text, int x, int y)
{
    TTF_Font* font = TTF_OpenFont("assets/font.ttf", 16);
    if (font == nullptr) std::cout << "Failed to load font! Error " << TTF_GetError();

    SDL_Color color = {0, 0, 0, 255};
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(Game::renderer, surface);
    SDL_Rect destRect = {x, y, surface->w, surface->h};
    SDL_RenderCopy(Game::renderer, texture, nullptr, &destRect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
    TTF_CloseFont(font);
}
