#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include "Entity.h"
#include "Enemy.h"
#include "Graphics.h"
#include <map>

enum class State {IDLE, WALK, CASTING, ATTACK, HEAL, HIT};

class Player : public Entity
{
public:
    Player(const char* file, const std::string &n, int hp, int atk, int x, int y);

    void handleEvents(SDL_Event &event);
    void update(Enemy* enemy);
    void render() override;

    void changeState(State state);

private:
    std::map<State, Animation> animations;
    State currentState = State::IDLE;
    int direction = 0;

};

#endif // PLAYER_H_INCLUDED
