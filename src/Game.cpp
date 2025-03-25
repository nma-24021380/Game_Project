#include "Game.h"
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>
#include "Player.h"
#include "Enemy.h"
#include "TileSet.h"
#include "Defs.h"
#include "SceneManager.h"
#include "Card.h"

Player* player = nullptr;
Enemy* enemy = nullptr;
Card* deck[TOTAL_CARDS] = {nullptr};

SDL_Renderer* Game::renderer = nullptr;
GameState currentState = GameState::MENU;

Game::Game()
{}

Game::~Game()
{}

void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen)
{
    int flags = 0;
    if (fullscreen) flags = SDL_WINDOW_FULLSCREEN;

    if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
    {
        std::cout << "SDL initialized!\n";
        window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
        if (window == nullptr)
        {
            std::cout << "Failed to create window! Error: " << SDL_GetError() << std::endl;
        }
        Game::renderer = SDL_CreateRenderer(window, -1, 0);
        if (Game::renderer == nullptr)
        {
            std::cout << "Failed to create renderer! Error: " << SDL_GetError() << std::endl;
        }
        isRunning = true;
    }
    else
    {
        std::cout << "Failed to initialize SDL! Error: " << SDL_GetError() << std::endl;
        isRunning = false;
    }
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
    {
        std::cout << "Failed to initialize SDL_image! Error: " << IMG_GetError() << std::endl;
    }
    if (TTF_Init() != 0)
    {
        std::cout << "Failed to initialize SDL_ttf! Error: " << TTF_GetError() << std::endl;
    }

    player = new Player("assets/player/idle.png", "Player", 20, 5, 0, 0);
    if (player == nullptr) std::cout << "Failed to initilaize player!";

    enemy = new Enemy("assets/enemy.png", "Duck", 20, 5, 100, 100);
    if (enemy == nullptr) std::cout << "Failed to initialize enemy!";

    setScene(SceneState::ADVENTURE);
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
                for (int i = 0; i < TOTAL_CARDS; i++)
                {
                    deck[i] = getRandomCard();
                    if (deck[i] == nullptr) std::cout << "Failed to initialize card!";
                }
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
        renderBattle(player, enemy);
        if (deck[0] != nullptr) renderDeck(deck);
    }
    enemy->render();
    player->render();

    SDL_RenderPresent(Game::renderer);
}

void Game::clean()
{
    if (window) SDL_DestroyWindow(window);
    if (Game::renderer) SDL_DestroyRenderer(Game::renderer);
    delete player;
    delete enemy;
    for (auto card : deck)
    {
        if (card == nullptr) delete card;
    }
    SDL_Quit();
    IMG_Quit();
    std::cout << "Game cleaned!\n";
}

bool Game::running()
{
    return isRunning;
}
