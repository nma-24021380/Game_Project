#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include "Entity.h"
#include "Enemy.h"
#include "TileSet.h"

class Player : public Entity
{
public:
    Player(const std::string &n, int hp, int atk, int x, int y);

    void handleEvents(TileSet* tileMap);

    void update(Enemy* enemy);
    void render();

private:
    int direction = 1;

};

#endif // PLAYER_H_INCLUDED
