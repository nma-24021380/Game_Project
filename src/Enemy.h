#ifndef ENEMY_H_INCLUDED
#define ENEMY_H_INCLUDED

#include "Entity.h"

class Enemy : public Entity
{
public:
    Enemy(const char* file, int hp, int atk, int x, int y);
    void update();

};

#endif // ENEMY_H_INCLUDED
