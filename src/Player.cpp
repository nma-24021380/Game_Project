#include "Player.h"
#include "Defs.h"
#include "SceneManager.h"
#include "Utils.h"
#include "Game.h"
#include <SDL_image.h>
#include <iostream>

Player::Player(const std::string &n, int hp, int atk, int x, int y)
: Entity(n, hp, atk, x, y)
{
    position = {x, y, PLAYER_WIDTH, PLAYER_HEIGHT};

    animations[State::IDLE] = Animation("assets/player/idle.png", 46, 88, 1, 1000);
    animations[State::WALK] = Animation("assets/player/walk.png", 46, 88, 4, 200);
    animations[State::BATTLE] = Animation("assets/player/battle.png", 88, 94, 5, 100);
    animations[State::HEAL] = Animation("assets/player/heal.png", 88, 94, 14, 100);
    animations[State::SHIELD] = Animation("assets/player/shield.png", 88, 94, 9, 100);
    animations[State::ATTACK] = Animation("assets/player/attack.png", 166, 94, 7, 100);

}

void Player::handleEvents(TileSet* tileMap)
{
    static Uint32 lastKeyPress = 0;
    Uint32 currentTime = SDL_GetTicks();

    if (getScene() == SceneState::ADVENTURE)
    {
        if ((Game::event.type == SDL_KEYDOWN) && (currentTime - lastKeyPress > 50))
        {
            int x = position.x;
            int y = position.y;

            switch(Game::event.key.keysym.sym)
            {
                case SDLK_UP:
                    direction = 0; changeState(State::WALK);
                    y -= PLAYER_SPEED;
                    break;
                case SDLK_DOWN:
                    direction = 1; changeState(State::WALK);
                    y += PLAYER_SPEED;
                    break;
                case SDLK_LEFT:
                    direction = 2; changeState(State::WALK);
                    x -= PLAYER_SPEED;
                    break;
                case SDLK_RIGHT:
                    direction = 3; changeState(State::WALK);
                    x += PLAYER_SPEED;
                    break;
            }
            if (atBorder(x, y))
            {
                changeMap(tileMap, x, y, direction);
                position.x = x;
                position.y = y;
            }
            else if (!checkCollision(tileMap, x, y))
            {
                position.x = x;
                position.y = y;
            }
            lastKeyPress = currentTime;
        }
        else if (Game::event.type == SDL_KEYUP)
        {
            changeState(State::IDLE);
        }
    }
}

void Player::update(Enemy* enemy)
{
    Entity::update();
    if (getScene() == SceneState::ADVENTURE)
    {
        if (checkEnemy(this->getPos(), enemy->getPos()))
        {
            changeScene(SceneState::BATTLE);
            changeState(State::BATTLE);
        }
    }
}

void Player::render()
{
    if (getScene() == SceneState::BATTLE)
    {
        animations[entity_state].render(250, SCREEN_HEIGHT / 2);
    }
    if (getScene() == SceneState::ADVENTURE)
    {
        animations[entity_state].render(position.x, position.y, direction);
    }
}
