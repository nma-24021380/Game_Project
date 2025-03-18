#ifndef CARD_H_INCLUDED
#define CARD_H_INCLUDED

#include <SDL.h>
#include "Enemy.h"

class Card
{
public:
    Card(const char* file);
    void render();
    void cardSelection(SDL_Event &event, Enemy* enemy);
    void useCard(int cardNum, Enemy* enemy);

private:
    SDL_Texture* cardSprite;
    SDL_Rect cardPos;

    int selectedCard = 0;
    const int numCards = 5;
    Uint32 lastKeyPress = 0;
};

#endif // CARD_H_INCLUDED
