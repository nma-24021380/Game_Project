#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include "Entity.h"
#include "TileSet.h"

class Enemy;

class Player : public Entity
{
public:
    Player(const std::string &n, int hp, int atk, int x, int y);

    void handleEvents();

    void update() override;
    void render();

    void restart();

private:
    int direction = 1;

};

extern Player* player;

#endif // PLAYER_H_INCLUDED
