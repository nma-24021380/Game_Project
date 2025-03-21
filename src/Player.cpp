#include "Player.h"
#include "Defs.h"
#include "SceneManager.h"
#include "Utils.h"

Player::Player(const char* file, int hp, int atk, int x, int y)
: Entity(file, hp, atk, x, y)
{}

void Player::handleEvents(SDL_Event &event)
{
    if (event.type == SDL_KEYDOWN)
    {
        switch(event.key.keysym.sym)
        {
            case SDLK_UP: position.y -= PLAYER_SPEED; break;
            case SDLK_DOWN: position.y += PLAYER_SPEED; break;
            case SDLK_LEFT: position.x -= PLAYER_SPEED; break;
            case SDLK_RIGHT: position.x += PLAYER_SPEED; break;
        }
    }
}

void Player::update(Enemy* enemy)
{
    if (getScene() == SceneState::BATTLE)
    {
        position.x = SCREEN_WIDTH - position.w - 50;
        position.y = SCREEN_HEIGHT / 2;
    }
    else if (getScene() == SceneState::ADVENTURE)
    {
        if (position.x < 0) position.x = 0;
        if (position.y < 0) position.y = 0;
        if (position.x > SCREEN_WIDTH - position.w) position.x = SCREEN_WIDTH - position.w;
        if (position.y > SCREEN_HEIGHT - position.h) position.y = SCREEN_HEIGHT - position.h;

        if (checkCollision(this->getBounds(), enemy->getBounds()))
        {
            setScene(SceneState::BATTLE);
        }
    }
}
