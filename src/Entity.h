#ifndef ENTITY_H_INCLUDED
#define ENTITY_H_INCLUDED

#include <SDL.h>
#include <string>

class Entity
{
public:
    Entity(const char* file, const std::string &n, int hp, int atk, int x, int y);
    virtual ~Entity();

    virtual void render();
    SDL_Rect getBounds();
    void takeDamage(int damage);
    void dealDamageTo(Entity* target);
    bool isAlive();


    SDL_Texture* sprite;
    SDL_Rect position;

    std::string name;
    int maxHealth;
    int health;
    int attack;

};

#endif // ENTITY_H_INCLUDED
