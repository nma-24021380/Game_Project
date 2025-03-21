#ifndef CARD_H_INCLUDED
#define CARD_H_INCLUDED

#include <SDL.h>
#include <string>
#include "Enemy.h"

enum CardType {ATTACK, HEAL};

const int TOTAL_CARDS = 6;

class Card
{
public:
    Card(CardType type, int star);
    SDL_Rect getPos();

    void render(SDL_Rect &destRect);
    void updateEffect();

    void useCard(int cardIndex, Enemy* enemy);

    bool canMerge(Card* other);
    void mergeWith(Card* other);

private:
    SDL_Texture* sprite;
    SDL_Rect position;

    CardType type;
    int star;
    std::string effect;

};

void renderDeck(Card* deck[]);

void handleCardEvents(SDL_Event &event, Enemy* enemy, Card* deck[]);

Card* getRandomCard();

#endif // CARD_H_INCLUDED
