#include "SceneManager.h"
#include "Adventure.h"
#include "Battle.h"
#include "Card.h"
#include "Game.h"
#include "Defs.h"
#include "Utils.h"
#include <iostream>

SDL_Texture* game_win = nullptr;

SDL_Texture* game_over = nullptr;

SceneState currentScene;

SceneState getScene()
{
    return currentScene;
}

void changeScene(SceneState scene)
{
    currentScene = scene;
}

void renderGameWin()
{
    if (!game_win) game_win = loadTexture("assets/game_win.png");

    if (game_win)
    {
        SDL_Delay(500);
        SDL_RenderCopy(Game::renderer, game_win, NULL, NULL);
    }
}

void renderGameOver()
{
    if (!game_over) game_over = loadTexture("assets/game_over.png");

    if (game_over)
    {
        SDL_Delay(500);
        SDL_RenderCopy(Game::renderer, game_over, NULL, NULL);
    }
}

void cleanScene()
{
    if (getScene() == SceneState::BATTLE)
    {
        std::cout << "Cleaning battle...\n";
        cleanBattle();
        std::cout << "Battle cleaned!\n";
    }

    cleanAdventure();
    std::cout << "Adventure cleaned!\n";

    if (game_over)
    {
        SDL_DestroyTexture(game_over);
        game_over = nullptr;
    }

    if (game_win)
    {
        SDL_DestroyTexture(game_win);
        game_win = nullptr;
    }
}
