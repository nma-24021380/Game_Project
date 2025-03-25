#include "Entity.h"
#include <SDL_image.h>
#include "Game.h"

Entity::Entity(const char* file, const std::string &n, int hp, int atk, int x, int y)
: name(n), maxHealth(hp), health(hp), attack(atk)
{
    sprite = IMG_LoadTexture(Game::renderer, file);
    position = {x, y, 64, 64};
}

Entity::~Entity()
{
    SDL_DestroyTexture(sprite);
}

void Entity::render()
{
    SDL_RenderCopy(Game::renderer, sprite, nullptr, &position);
}

SDL_Rect Entity::getBounds()
{
    return {position.x, position.y, position.w, position.h};
}

void Entity::takeDamage(int damage)
{
    health -= damage;
    if (health < 0) health = 0;
}

void Entity::dealDamageTo(Entity* target)
{
    target->takeDamage(this->attack);
}

bool Entity::isAlive()
{
    return health > 0;
}
