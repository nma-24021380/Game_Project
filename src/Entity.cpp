#include "Entity.h"
#include "Game.h"
#include <SDL_image.h>
#include <iostream>

Entity::Entity(const std::string &n, int hp, int atk, int x, int y)
: name(n), max_health(hp), health(hp), attack(atk)
{
    position = {x, y, 64, 64};
}

Entity::~Entity()
{}

SDL_Rect Entity::getPos()
{
    return position;
}

void Entity::changeState(State new_state)
{
    if (entity_state != new_state)
    {
        entity_state = new_state;
        animations[entity_state].start();
    }
}

void Entity::update()
{
    if (!animations[entity_state].isPlaying()) animations[entity_state].start();

    if (entity_state == State::HEAL || entity_state == State::ATTACK || entity_state == State::SHIELD)
    {
        animations[entity_state].update(false);
        if (!animations[entity_state].isPlaying()) changeState(State::BATTLE);
        // heal animation chi chay 1 lan
    }
    else
    {
        animations[entity_state].update();
    }
}

void Entity::takeDamage(int damage)
{
    health -= damage;
    if (health <= 0) health = 0;
}

void Entity::heal(int heal_amount)
{
    health += heal_amount;
    if (health > max_health) health = max_health;
}

bool Entity::isAlive()
{
    return health > 0;
}
