#include "Game.h"
#include "Player.h"
#include "Enemy.h"
#include "TileSet.h"
#include "SceneManager.h"
#include "Card.h"
#include "Battle.h"
#include "Adventure.h"
#include "Defs.h"
#include "Utils.h"
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <iostream>
#include <ctime>
#include <cstdlib>

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;

Game::Game()
{
    srand(time(0));
    is_running = false;
    needs_cleanup = false;
    window = nullptr;
}

Game::~Game()
{
    if (needs_cleanup) clean();
}

void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen)
{
    int flags = 0;
    if (fullscreen) flags = SDL_WINDOW_FULLSCREEN;

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        std::cout << "Failed to initialize SDL! Error: " << SDL_GetError() << std::endl;
        is_running = false;
        return;
    }

    window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
    if (!window)
    {
        std::cout << "Failed to create window! Error: " << SDL_GetError() << std::endl;
        is_running = false;
        return;
    }
    Game::renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!Game::renderer)
    {
        std::cout << "Failed to create renderer! Error: " << SDL_GetError() << std::endl;
        is_running = false;
        return;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
    {
        std::cout << "Failed to initialize SDL_image! Error: " << IMG_GetError() << std::endl;
        is_running = false;
        return;
    }
    if (TTF_Init() != 0)
    {
        std::cout << "Failed to initialize SDL_ttf! Error: " << TTF_GetError() << std::endl;
        is_running = false;
        return;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        std::cout << "Failed to initialize SDL_mixer! Error: " << Mix_GetError() << std::endl;
        is_running = false;
        return;
    }

    loadMusic();

    changeScene(SceneState::ADVENTURE);
    game_state = GameState::ADVENTURE;
    startAdventure();

    is_running = true;
    needs_cleanup = true;
}

void Game::handleEvents()
{
    while(SDL_PollEvent(&Game::event))
    {
        if (Game::event.type == SDL_QUIT)
        {
            if (game_state == GameState::BATTLE)
            {
                cleanBattle();
            }
            is_running = false;
            return;
        }

        if (getScene() == SceneState::ADVENTURE)
        {
            player->handleEvents();
            handleAdventure();
        }
        else if (getScene() == SceneState::BATTLE && current_enemy)
        {
            handlePlayerTurn();
            handleBattle();
        }
    }
}

void Game::update()
{
    if (!is_running) return;

    if (getScene() == SceneState::BATTLE && game_state != GameState::BATTLE)
    {
        game_state = GameState::BATTLE;
        startBattle(current_enemy);
    }
    else if (getScene() == SceneState::ADVENTURE && game_state != GameState::ADVENTURE)
    {
        game_state = GameState::ADVENTURE;
        endBattle();
        startAdventure();

        if (player)
        {
            player->changeState(State::IDLE);
            player->clearStatusEffects();
        }
    }

    if (game_state == GameState::ADVENTURE)
    {
        static int last_map_x = -1;
        static int last_map_y = -1;

        if (last_map_x != tile_map->getMapX() || last_map_y != tile_map->getMapY())
        {
            last_map_x = tile_map->getMapX();
            last_map_y = tile_map->getMapY();

            std::pair<int, int> current_map = {last_map_x, last_map_y};
            bool spawned = map_spawned.find(current_map) != map_spawned.end();

            if (!spawned)
            {
                Enemy* enemy = Enemy::spawnRandomEnemy();
                if (enemy) enemies.push_back(enemy);
            }
        }
    }

    if (game_state == GameState::BATTLE && current_enemy)
    {
        current_enemy->update();
        updatePlayerTurn();
        updateEnemyTurn();
    }

    if (player)
    {
        player->update();

        if (game_state == GameState::ADVENTURE)
        {
            for (auto enemy : enemies)
            {
                if (enemy && enemy->isAlive() && checkEntity(player->getPos(), enemy->getPos()))
                {
                    changeScene(SceneState::BATTLE);
                    startBattle(enemy);
                    break;
                }
            }

            if (checkWinning()) changeScene(SceneState::WIN);
        }

        if (!player->isAlive()) changeScene(SceneState::GAME_OVER);
    }

    if (game_state == GameState::ADVENTURE)
    {
        for (auto enemy : enemies)
        {
            if (enemy) enemy->update();
        }
    }
}

void Game::render()
{
    if (!is_running) return;

    SDL_RenderClear(Game::renderer);

    if (getScene() == SceneState::ADVENTURE)
    {
        renderAdventure();
    }
    else if (getScene() == SceneState::BATTLE && current_enemy)
    {
        renderBattle();
    }

    if (getScene() == SceneState::GAME_OVER)
    {
        renderGameOver();
    }
    else if (getScene() == SceneState::WIN)
    {
        renderGameWin();
    }

    SDL_RenderPresent(Game::renderer);
}

void Game::clean()
{
    if (!needs_cleanup) return;

    if (Mix_PlayingMusic())
    {
        Mix_HaltMusic();
        std::cout << "Music stopped!\n";
    }

    cleanMusic();
    std::cout << "Music cleaned!\n";

    for (auto it = enemies.begin(); it != enemies.end();)
    {
        if (*it)
        {
            delete *it;
            *it = nullptr;
        }
        it = enemies.erase(it);
    }
    enemies.clear();
    std::cout << "Enemies cleared!\n";

    if (current_enemy)
    {
        delete current_enemy;
        current_enemy = nullptr;
    }
    std::cout << "Current enemy cleared!\n";

    cleanScene();

    map_spawned.clear();

    if (Game::renderer)
    {
        SDL_DestroyRenderer(Game::renderer);
        Game::renderer = nullptr;
    }
    std::cout << "Renderer cleared!\n";

    if (window)
    {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    std::cout << "Window cleared!\n";

    Mix_Quit();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

    is_running = false;
    needs_cleanup = false;
    std::cout << "Game cleaned!\n";
}

bool Game::running()
{
    return is_running;
}
