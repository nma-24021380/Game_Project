#include "Card.h"
#include <SDL_image.h>
#include <iostream>
#include "Game.h"
#include "Defs.h"

int selectedCard = 0;
Uint32 lastKeyPress = 0;

Card::Card(CardType t, int s) : type(t), star(s)
{
    position.w = 64;
    position.h = 96;

    const char* file;
    if (type == ATTACK)
    {
        switch(star)
        {
            case 1: file = "assets/card_attack01.png"; break;
            case 2: file = "assets/card_attack02.png"; break;
            case 3: file = "assets/card_debuff.png"; break;
        }

    }
    else if (type == HEAL)
    {
        switch(star)
        {
            case 1: file = "assets/card_heal01.png"; break;
            case 2: file = "assets/card_heal02.png"; break;
            case 3: file = "assets/card_shield.png"; break;
        }
    }
    sprite = IMG_LoadTexture(Game::renderer, file);
    if (sprite == NULL)
    {
        std::cout << "Failed to load card sprite!" << IMG_GetError() << std::endl;
    }

    updateEffect();
}

void Card::updateEffect()
{
    if (type == ATTACK)
    {
        switch(star)
        {
            case 1: effect = "Basic Attack"; break;
            case 2: effect = "Upgraded Attack"; break;
            case 3: effect = "Stun Debuff"; break;
        }
    }
    else if (type == HEAL)
    {
        switch(star)
        {
            case 1: effect = "Basic Heal"; break;
            case 2: effect = "Upgraded Heal"; break;
            case 3: effect = "Shield"; break;
        }
    }
}

SDL_Rect Card::getPos()
{
    return position;
}

Card* getRandomCard()
{
    CardType type = (rand() % 2) ? ATTACK : HEAL;
    int star = 1;
    return new Card(type, star);
}

void Card::render(SDL_Rect &destRect)
{
    SDL_RenderCopy(Game::renderer, sprite, NULL, &destRect);
}

void renderDeck(Card* deck[])
{
    for (int i = 0; i < TOTAL_CARDS; i++)
    {
        int w = deck[i]->getPos().w;
        int h = deck[i]->getPos().h;
        SDL_Rect destRect = {64 + i * (w + 16), SCREEN_HEIGHT - h - 64, w, h};
        if (i == selectedCard)
        {
            SDL_SetRenderDrawColor(Game::renderer, 255, 255, 255, 255);
            SDL_RenderFillRect(Game::renderer, &destRect);
            SDL_SetRenderDrawColor(Game::renderer, 0, 0, 0, 0);
            // de tam hinh trang
        }
        else
        {
            deck[i]->render(destRect);
        }
    }
}

void handleCardEvents(SDL_Event &event, Enemy* enemy, Card* deck[])
{
    Uint32 currentTime = SDL_GetTicks();
    if ((event.type == SDL_KEYDOWN) && (currentTime - lastKeyPress > 150))
    {
        switch(event.key.keysym.sym)
        {
            case SDLK_LEFT: selectedCard = (selectedCard - 1 + TOTAL_CARDS) % TOTAL_CARDS; break;
            case SDLK_RIGHT: selectedCard = (selectedCard + 1) % TOTAL_CARDS; break;
            case SDLK_RETURN: deck[selectedCard]->useCard(selectedCard, enemy); break;
            case SDLK_SPACE:
                if (selectedCard < TOTAL_CARDS - 1 && deck[selectedCard]->canMerge(deck[selectedCard + 1]))
                {
                    deck[selectedCard]->mergeWith(deck[selectedCard + 1]);
                }
        }
        lastKeyPress = currentTime;
    }
}

void Card::useCard(int cardIndex, Enemy* enemy)
{
    enemy->takeDamage(5);
    std::cout << "Use card number " << cardIndex << std::endl;
    std::cout << "Enemy HP: " << enemy->health << std::endl;
}

bool Card::canMerge(Card* other)
{
    return (other != NULL && type == other->type && star == other->star && star < 3);
}

void Card::mergeWith(Card* other)
{
    if (canMerge(other))
    {
        star++;
        updateEffect();
        other->star = 0;
    }
}
