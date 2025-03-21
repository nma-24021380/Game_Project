#include "Game.h"
#include <SDL_image.h>
#include <iostream>
#include "Player.h"
#include "Enemy.h"
#include "TileSet.h"
#include "Defs.h"
#include "SceneManager.h"
#include "Card.h"

Player* player = NULL;
Enemy* enemy = NULL;
Card* deck[TOTAL_CARDS];

SDL_Renderer* Game::renderer = NULL;
GameState currentState = GameState::MENU;

Game::Game()
{}

Game::~Game()
{}

void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen)
{
    int flags = 0;
    if (fullscreen)
    {
        flags = SDL_WINDOW_FULLSCREEN;
    }
    if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
    {
        std::cout << "Successfully initialized!\n";
        window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
        if (window)
        {
            std::cout << "Window created!\n";
        }
        Game::renderer = SDL_CreateRenderer(window, -1, 0);
        if (Game::renderer)
        {
            SDL_SetRenderDrawColor(Game::renderer, 255, 155, 255, 255);
            std::cout << "Renderer created!\n";
        }
        isRunning = true;
    }
    else
    {
        isRunning = false;
    }
    player = new Player("assets/player.png", 20, 5, 0, 0);
    enemy = new Enemy("assets/enemy.png", 20, 5, 100, 100);


}

void Game::handleEvents()
{
    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            isRunning = false;
            return;
        }
        player->handleEvents(event);

        if (getScene() == SceneState::BATTLE)
        {
            if (currentState != GameState::BATTLE)
            {
                currentState = GameState::BATTLE;
                for (int i = 0; i < TOTAL_CARDS; i++) deck[i] = getRandomCard();
            }
            handleCardEvents(event, enemy, deck);
        }
    }
}


void Game::update()
{
    player->update(enemy);
    enemy->update();
}

void Game::render()
{
    SDL_RenderClear(Game::renderer);

    if (getScene() == SceneState::ADVENTURE)
    {
        renderAdventure();
    }
    else
    {
        renderBattle();
        renderDeck(deck);
    }
    enemy->render();
    player->render();

    SDL_RenderPresent(Game::renderer);
}

void Game::clean()
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(Game::renderer);
    delete player;
    delete enemy;
    for (auto card : deck)
    {
        delete card;
    }
    SDL_Quit();
    IMG_Quit();
    std::cout << "Game cleaned!\n";
}

bool Game::running()
{
    return isRunning;
}
