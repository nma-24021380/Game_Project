#include "Entity.h"
#include "Game.h"
#include "Utils.h"
#include <SDL_image.h>
#include <iostream>

Entity::Entity(const std::string &n, int hp, int atk, int x, int y)
: name(n), max_health(hp), health(hp), attack(atk)
{
    position = {x, y, 64, 64};
    explosion = Animation("assets/explosion.png", 96, 96, 12, 100);
    stun = Animation("assets/stunned.png", 50, 50, 3, 100);
}

Entity::~Entity()
{}

SDL_Rect Entity::getPos()
{
    return position;
}

void Entity::changeState(State new_state)
{
    if (entity_state != new_state || !animations[entity_state].isPlaying())
    {
        entity_state = new_state;
        animations[entity_state].start();
    }
}

void Entity::update()
{
    if (!animations[entity_state].isPlaying())
    {
        animations[entity_state].start();
    }

    if (entity_state == State::HIT && !explosion.isPlaying())
    {
        explosion.start();
    }

    if (isStunned() && !stun.isPlaying())
    {
        stun.start();
    }

    if (entity_state == State::HEAL || entity_state == State::ATTACK || entity_state == State::SHIELDED)
    {
        animations[entity_state].update(false);     // chi chay animation 1 lan

        if (!animations[entity_state].isPlaying())  // chay xong thi chuyen sang state khac
        {
            changeState(State::BATTLE);
        }
    }
    else if (entity_state == State::HIT)
    {
        animations[entity_state].update(false);
        explosion.update(false);

        if (!animations[entity_state].isPlaying())
        {
            if (!isAlive()) changeState(State::IDLE);
            else changeState(State::BATTLE);
        }
    }
    else
    {
        animations[entity_state].update(true);  // chay animation lien tuc
    }
}

void Entity::takeDamage(int damage)
{
    if (shielded)
    {
        shielded = false;
        return;
    }

    health -= damage;
    if (health <= 0) health = 0;

    if (explosion_sfx) Mix_PlayChannel(-1, explosion_sfx, 0);

    if (isAlive())
    {
        changeState(State::HIT);
        std::cout << "Entity hit! Health: " << health << std::endl;
    }
}

void Entity::heal(int heal_amount)
{
    health += heal_amount;
    if (health > max_health) health = max_health;

    if (heal_sfx) Mix_PlayChannel(-1, heal_sfx, 0);
}

void Entity::applyStun()
{
    stunned = true;
}

void Entity::applyShield()
{
    shielded = true;
    changeState(State::SHIELDED);
}

void Entity::clearStatusEffects()
{
    stunned = false;
    shielded = false;
    changeState(State::BATTLE);
}

bool Entity::isAlive() { return health > 0; }

bool Entity::isStunned() { return stunned; }

bool Entity::isShielded() { return shielded; }
