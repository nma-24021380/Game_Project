#ifndef ENEMY_H_INCLUDED
#define ENEMY_H_INCLUDED

#include "Entity.h"

class Enemy : public Entity
{
public:
    Enemy(const std::string &n, int hp, int atk, int x, int y);

    void update() override;
    void render();

};

#endif // ENEMY_H_INCLUDED
