#include "Enemy.h"
#include "SceneManager.h"
#include "Defs.h"

Enemy::Enemy(const std::string &n, int hp, int atk, int x, int y)
: Entity(n, hp, atk, x, y)
{
    animations[State::IDLE] = Animation("assets/enemy.png", 50, 50, 1, 1000);
}

void Enemy::update()
{
    Entity::update();
    if (!isAlive())
    {
        changeScene(SceneState::ADVENTURE);
    }
}

void Enemy::render()
{
    if (getScene() == SceneState::BATTLE)
    {
        animations[entity_state].render(SCREEN_WIDTH - position.w - 250, SCREEN_HEIGHT / 2);
    }
    else if (getScene() == SceneState::ADVENTURE)
    {
        animations[entity_state].render(position.x, position.y);
    }
}
