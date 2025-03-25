#include "Player.h"
#include "Defs.h"
#include "SceneManager.h"
#include "Utils.h"
#include "Game.h"
#include <SDL_image.h>
#include <iostream>

const int PLAYER_SPEED = 20;

Player::Player(const char* file, const std::string &n, int hp, int atk, int x, int y)
: Entity(file, n, hp, atk, x, y)
{
    sprite = IMG_LoadTexture(Game::renderer, "assets/player/idle.png");
    position = {x, y, 23, 44};

    animations[State::IDLE] = Animation("assets/player/idle.png", 23, 44, 1, 100);
    animations[State::WALK] = Animation("assets/player/walk.png", 23, 44, 4, 100);
    animations[State::CASTING] = Animation("assets/player/casting.png", 44, 47, 6, 100);
    animations[State::HEAL] = Animation("assets/player/heal.png", 44, 47, 8, 100);

}

void Player::handleEvents(SDL_Event &event)
{
    static Uint32 lastKeyPress = 0;
    Uint32 currentTime = SDL_GetTicks();
    if ((getScene() == SceneState::ADVENTURE) && (event.type == SDL_KEYDOWN) && (currentTime - lastKeyPress > 200))
    {
        switch(event.key.keysym.sym)
        {
            case SDLK_UP:
                direction = 0; currentState = State::WALK;
                position.y -= PLAYER_SPEED;
                break;
            case SDLK_DOWN:
                direction = 1; currentState = State::WALK;
                position.y += PLAYER_SPEED;
                break;
            case SDLK_LEFT:
                direction = 2; currentState = State::WALK;
                position.x -= PLAYER_SPEED;
                break;
            case SDLK_RIGHT:
                direction = 3; currentState = State::WALK;
                position.x += PLAYER_SPEED;
                break;
        }
        lastKeyPress = currentTime;
    }
    else if (event.type == SDL_KEYUP)
    {
        changeState(State::IDLE);
    }
}

void Player::update(Enemy* enemy)
{
    if (getScene() == SceneState::BATTLE)
    {
        position.x = 50;
        position.y = SCREEN_HEIGHT / 2;
        if (currentState == State::CASTING)
        {
            if (!animations[State::CASTING].isPlaying()) animations[State::CASTING].start();
            animations[State::CASTING].update();
            if (!animations[State::CASTING].isPlaying()) changeState(State::IDLE);
        }
    }
    else if (getScene() == SceneState::ADVENTURE)
    {
        if (position.x < 0) position.x = 0;
        if (position.y < 0) position.y = 0;
        if (position.x > SCREEN_WIDTH - position.w) position.x = SCREEN_WIDTH - position.w;
        if (position.y > SCREEN_HEIGHT - position.h) position.y = SCREEN_HEIGHT - position.h;

        if (currentState == State::WALK)
        {
            if (!animations[State::WALK].isPlaying()) animations[State::WALK].start();
            animations[State::WALK].update();
        }

        if (checkCollision(this->getBounds(), enemy->getBounds()))
        {
            setScene(SceneState::BATTLE);
        }
    }
}

void Player::render()
{
    if (currentState == State::IDLE)
    {
        animations[State::IDLE].render(position.x, position.y);
    }
    else if (currentState == State::WALK)
    {
        animations[State::WALK].render(position.x, position.y, direction);
    }
    else if (currentState == State::CASTING)
    {
        animations[State::CASTING].render(position.x, position.y);
    }
}

void Player::changeState(State state)
{
    if (currentState != state)
    {
        currentState = state;
        animations[currentState].start();
    }
}
