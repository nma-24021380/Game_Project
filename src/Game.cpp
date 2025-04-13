#include "Game.h"
#include "Player.h"
#include "Enemy.h"
#include "TileSet.h"
#include "Defs.h"
#include "SceneManager.h"
#include "Card.h"
#include "Battle.h"
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>

Player* player = nullptr;
Enemy* enemy = nullptr;
TileSet* tileMap = nullptr;

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;
GameState game_state = GameState::ADVENTURE;

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

    player = new Player("Player", 100, 5, 200, 300);
    if (player == nullptr) std::cout << "Failed to initilaize player!";

    enemy = new Enemy("Duck", 100, 5, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
    if (enemy == nullptr) std::cout << "Failed to initialize enemy!";

    tileMap = new TileSet("assets/tileset.png");

    changeScene(SceneState::ADVENTURE);
}

void Game::handleEvents()
{
    while(SDL_PollEvent(&Game::event))
    {
        if (Game::event.type == SDL_QUIT)
        {
            isRunning = false;
            return;
        }
        else if (getScene() == SceneState::ADVENTURE)
        {
            player->handleEvents(tileMap);
        }
        else if (getScene() == SceneState::BATTLE)
        {
            handlePlayerTurn(player, enemy, deck);
        }
    }
}

void Game::update()
{
    if (getScene() == SceneState::BATTLE && game_state != GameState::BATTLE)
    {
        game_state = GameState::BATTLE;
        startBattle();
    }
    else if (getScene() == SceneState::ADVENTURE && game_state != GameState::ADVENTURE)
    {
        game_state = GameState::ADVENTURE;
        endBattle();
    }
    player->update(enemy);
    enemy->update();
    updatePlayerTurn(player, enemy, deck);
    updateEnemyTurn(player, enemy, deck);
}

void Game::render()
{
    SDL_RenderClear(Game::renderer);

    if (getScene() == SceneState::ADVENTURE)
    {
        renderAdventure(tileMap);
    }
    else if (getScene() == SceneState::BATTLE)
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
    delete tileMap;

    SDL_Quit();
    IMG_Quit();
    std::cout << "Game cleaned!\n";
}

bool Game::running()
{
    return isRunning;
}
