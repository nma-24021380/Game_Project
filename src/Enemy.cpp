#include "Enemy.h"
#include "SceneManager.h"
#include "Defs.h"

Enemy::Enemy(const char* file, const std::string &n, int hp, int atk, int x, int y)
: Entity(file, n, hp, atk, x, y)
{}

void Enemy::update()
{
    if (getScene() == SceneState::BATTLE)
    {
        position.x = SCREEN_WIDTH - position.w - 50;
        position.y = SCREEN_HEIGHT / 2;

        if (!(this->isAlive()))
        {
            setScene(SceneState::ADVENTURE);
        }
    }

}
