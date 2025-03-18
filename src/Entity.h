#ifndef ENTITY_H_INCLUDED
#define ENTITY_H_INCLUDED

#include <SDL.h>

class Entity
{
public:
    Entity(const char* file, int hp, int atk, int x, int y);

    void render();
    SDL_Rect getBounds();
    void takeDamage(int damage);
    void dealDamageTo(Entity* target);
    bool isAlive();


    SDL_Texture* sprite;
    SDL_Rect position;

    int health;
    int attack;

};

#endif // ENTITY_H_INCLUDED
