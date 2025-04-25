#include "Player.h"
#include "Enemy.h"
#include "Defs.h"
#include "SceneManager.h"
#include "Utils.h"
#include "Game.h"
#include <SDL_image.h>

Player* player = nullptr;

Player::Player(const std::string &n, int hp, int atk, int x, int y)
: Entity(n, hp, atk, x, y)
{
    position = {x, y, PLAYER_WIDTH, PLAYER_HEIGHT};

    animations[State::IDLE] = Animation("assets/player/idle.png", 46, 88, 1, 1000);
    animations[State::WALK] = Animation("assets/player/walk.png", 46, 88, 4, 200);
    animations[State::BATTLE] = Animation("assets/player/battle.png", 88, 94, 5, 100);
    animations[State::HEAL] = Animation("assets/player/heal.png", 88, 94, 14, 100);
    animations[State::SHIELDED] = Animation("assets/player/shield.png", 96, 94, 9, 100);
    animations[State::ATTACK] = Animation("assets/player/attack.png", 166, 94, 7, 70);
    animations[State::HIT] = Animation("assets/player/hit.png", 107, 94, 5, 200);

}

void Player::handleEvents()
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
                tile_map->changeMap(x, y, direction);
                position.x = x;
                position.y = y;
            }
            else if (!checkCollision(position.w, position.h, x, y))
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

void Player::update()
{
    Entity::update();

    if (getScene() == SceneState::ADVENTURE && !enemies.empty())
    {
        for (auto enemy : enemies)
        {
            if (checkEntity(this->getPos(), enemy->getPos()))
            {
                changeState(State::BATTLE);
                enemy->changeState(State::BATTLE);
                break;
            }
        }
    }
}

void Player::render()
{
    if (getScene() == SceneState::BATTLE)
    {
        animations[entity_state].render(PLAYER_BATTLE_X, PLAYER_BATTLE_Y);

        if (isStunned())
        {
            stun.render(PLAYER_BATTLE_X, PLAYER_BATTLE_Y);
        }

        if (entity_state == State::HIT)
        {
            explosion.render(PLAYER_BATTLE_X, PLAYER_BATTLE_Y);
        }
    }
    if (getScene() == SceneState::ADVENTURE)
    {
        animations[entity_state].render(position.x, position.y, direction);
    }
}
