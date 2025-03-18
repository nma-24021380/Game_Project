#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include "Entity.h"
#include "Enemy.h"

class Player : public Entity
{
public:
    Player(const char* file, int hp, int atk, int x, int y);

    void handleEvents(SDL_Event &event);
    void update(Enemy* enemy);

};

#endif // PLAYER_H_INCLUDED
