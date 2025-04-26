#include "Enemy.h"
#include "Card.h"
#include "SceneManager.h"
#include "TileSet.h"
#include "Utils.h"
#include "Defs.h"
#include <algorithm>
#include <iostream>

std::vector<Enemy*> enemies;
Enemy* current_enemy = nullptr;
std::set<std::pair<int, int>> map_spawned;

Enemy::Enemy(const std::string &n, int hp, int atk, int x, int y, int map_x, int map_y)
: Entity(n, hp, atk, x, y), map_x(map_x), map_y(map_y)
{
    animations[State::IDLE] = Animation("assets/enemy/idle.png", 46, 88, 1, 1000);
    animations[State::BATTLE] = Animation("assets/enemy/battle.png", 166, 94, 5, 100);
    animations[State::HEAL] = Animation("assets/enemy/heal.png", 166, 94, 14, 100);
    animations[State::SHIELDED] = Animation("assets/enemy/shield.png", 166, 94, 9, 100);
    animations[State::ATTACK] = Animation("assets/enemy/attack.png", 166, 94, 7, 70);
    animations[State::HIT] = Animation("assets/enemy/hit.png", 166, 94, 5, 200);
    enemies.push_back(this);
}

Enemy::~Enemy()
{
    for (int i = 0; i < 2; ++i)
    {
        if (e_deck[i])
        {
            delete e_deck[i];
            e_deck[i] = nullptr;
        }
    }

    auto it = std::find(enemies.begin(), enemies.end(), this);
    if (it != enemies.end()) enemies.erase(it);
}

void Enemy::addAttack()
{
    ++attack;
}

Enemy* Enemy::spawnRandomEnemy()
{
    if (getScene() != SceneState::ADVENTURE) return nullptr;

    Enemy* new_enemy = nullptr;
    while (!new_enemy)
    {
        int x = rand() % (MAP_WIDTH * TILE_SIZE);
        int y = rand() % (MAP_HEIGHT * TILE_SIZE);

        if (availablePosition(x, y))
        {
            new_enemy = new Enemy("...", 100, 2, x, y, tile_map->getMapX(), tile_map->getMapY());
            map_spawned.insert({tile_map->getMapX(), tile_map->getMapY()});
        }
    }
    return new_enemy;
}

void Enemy::spawnEnemies()
{
    static int last_map_x = -1;
    static int last_map_y = -1;

    if (last_map_x != tile_map->getMapX() || last_map_y != tile_map->getMapY())
    {
        last_map_x = tile_map->getMapX();
        last_map_y = tile_map->getMapY();

        std::pair<int, int> current_map = {last_map_x, last_map_y};
        bool spawned = map_spawned.find(current_map) != map_spawned.end();

        if (!spawned)
        {
            Enemy* enemy = Enemy::spawnRandomEnemy();
            if (enemy) enemies.push_back(enemy);
        }
    }
}

void Enemy::update()
{
    Entity::update();

    if (getScene() == SceneState::ADVENTURE)
    {
        if (checkEntity(player->getPos(), this->getPos())
            && tile_map->getMapX() == map_x && tile_map->getMapY() == map_y)
        {
            changeState(State::BATTLE);
        }
    }
}

void Enemy::render()
{
    if (getScene() == SceneState::BATTLE && this == current_enemy)
    {
        animations[entity_state].render(ENEMY_BATTLE_X, ENEMY_BATTLE_Y);

        if (isStunned())
        {
            stun.render(ENEMY_BATTLE_X + 50, ENEMY_BATTLE_Y);
        }

        if (entity_state == State::HIT)
        {
            explosion.render(ENEMY_BATTLE_X + 75, ENEMY_BATTLE_Y);
        }
    }
    else if (getScene() == SceneState::ADVENTURE)
    {
        if (map_x == tile_map->getMapX() && map_y == tile_map->getMapY())
        {
            animations[entity_state].render(position.x, position.y);
        }
    }
}

void Enemy::refillEnemyDeck()
{
    for (int i = 0; i < 2; ++i)
    {
        if (e_deck[i])
        {
            delete e_deck[i];
            e_deck[i] = nullptr;
        }

        switch(i)
        {
        case 0:
            if (health > max_health * 0.5)
            {
                e_deck[i] = new Card(ATTACK, 1);
            }
            else
            {
                e_deck[i] = getRandomCard();
            }
            break;
        case 1:
            if (health < max_health * 0.1)
            {
                e_deck[i] = new Card(ATTACK, 2);
            }
            else if (health < max_health * 0.2)
            {
                e_deck[i] = new Card(HEAL, 3);
            }
            break;
        default:
            e_deck[i] = getRandomCard();
            break;
        }
    }
}

void Enemy::useCard(int index)
{
    if (e_deck[index]) e_deck[index]->useCard(this, player);
}

void Enemy::restart()
{
    for (auto it = enemies.begin(); it != enemies.end();)
    {
        if (*it)
        {
            delete *it;
            *it = nullptr;
        }
        it = enemies.erase(it);
    }
    enemies.clear();

    if (current_enemy)
    {
        current_enemy = nullptr;
    }

    map_spawned.clear();

    Enemy::spawnEnemies();
}
