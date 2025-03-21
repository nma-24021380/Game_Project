#include "Enemy.h"
#include "SceneManager.h"
#include "Defs.h"

Enemy::Enemy(const char* file, int hp, int atk, int x, int y)
: Entity(file, hp, atk, x, y)
{}

void Enemy::update()
{
    if (getScene() == SceneState::BATTLE)
    {
        position.x = 50;
        position.y = SCREEN_HEIGHT / 2;

        if (!(this->isAlive()))
        {
            setScene(SceneState::ADVENTURE);
        }
    }

}
