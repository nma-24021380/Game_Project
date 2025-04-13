#ifndef ENTITY_H_INCLUDED
#define ENTITY_H_INCLUDED

#include <SDL.h>
#include <string>
#include <map>
#include "Graphics.h"

enum class State {IDLE, WALK, BATTLE, ATTACK, HEAL, SHIELD, HIT};

class Entity
{
public:
    Entity(const std::string &n, int hp, int atk, int x, int y);
    virtual ~Entity();

    SDL_Rect getPos();
    void changeState(State new_state);

    //virtual void render();
    virtual void update();

    void takeDamage(int damage);
    void heal(int heal_amount);
    bool isAlive();

    std::string name;
    int max_health;
    int health;
    int attack;

protected:
    SDL_Rect position;
    State entity_state = State::IDLE;
    std::map<State, Animation> animations;

};

#endif // ENTITY_H_INCLUDED
