#ifndef CARD_H_INCLUDED
#define CARD_H_INCLUDED

#include <SDL.h>
#include <string>
#include "Enemy.h"
#include "Player.h"

enum CardType {ATTACK, HEAL, SHIELD, STUN};

const int TOTAL_CARDS = 6;
extern int valid_cards;

class Card
{
public:
    Card(CardType type, int star);
    Card(const Card &other);
    ~Card();

    void updateCard();

    void useCard(Entity* user, Entity* target);

    void removeCard(int index);

    bool canMerge(Card* other);
    void mergeWith(Card* other);

    void render(SDL_Rect &destRect, int index, bool is_selecting);

private:
    SDL_Texture* sprite;

    CardType type;
    int star;

};

extern Card* deck[TOTAL_CARDS];

void renderDeck();
void refillCardDeck();

Card* getRandomCard();

void mergeCards(int index);

#endif // CARD_H_INCLUDED
