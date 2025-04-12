#ifndef CARD_H_INCLUDED
#define CARD_H_INCLUDED

#include <SDL.h>
#include <string>
#include "Enemy.h"
#include "Player.h"

enum CardType {ATTACK, HEAL, SHIELD, DEBUFF};

const int TOTAL_CARDS = 6;
extern int valid_cards;

class Card
{
public:
    Card(CardType type, int star);
    ~Card();

    void updateCard();

    void useCard(Entity* user, Entity* target);

    void removeCard(int index, Card* deck[]);

    bool canMerge(Card* other);
    void mergeWith(Card* other, Card* deck[]);

    void render(SDL_Rect &destRect, int index, bool selecting);

private:
    SDL_Texture* sprite;
    int blinkTimer = 0;

    CardType type;
    int star;
    //std::string effect;

};

void renderDeck(Card* deck[]);

Card* getRandomCard();

#endif // CARD_H_INCLUDED
