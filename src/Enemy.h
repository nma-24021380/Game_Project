#ifndef ENEMY_H_INCLUDED
#define ENEMY_H_INCLUDED

#include "Entity.h"
#include "Defs.h"
#include "TileSet.h"
#include <vector>
#include <set>

class Player;

class Card;

class Enemy : public Entity
{
public:
    Enemy(const std::string &n, int hp, int atk, int x, int y, int map_x, int map_y);
    ~Enemy();

    void addAttack();

    static Enemy* spawnRandomEnemy();

    void update() override;
    void render();

    void refillEnemyDeck();
    void useCard(int index);

    int map_x, map_y;

private:
    Card* e_deck[2] = {nullptr};

};

extern std::vector<Enemy*> enemies;
extern Enemy* current_enemy;
extern std::set<std::pair<int, int>> map_spawned;

#endif // ENEMY_H_INCLUDED
