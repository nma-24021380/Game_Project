#include "Card.h"
#include <SDL_image.h>
#include <iostream>
#include "Game.h"
#include "Defs.h"

int validCards = TOTAL_CARDS;
int selected = 0;

Card::Card(CardType t, int s) : type(t), star(s)
{
    position.w = 64;
    position.h = 96;

    updateEffect();
    updateTexture();
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

void Card::updateTexture()
{
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
    if (sprite == nullptr)
    {
        std::cout << "Failed to load card sprite!" << IMG_GetError() << std::endl;
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

void Card::render(SDL_Rect &destRect, bool isSelected)
{
    SDL_RenderCopy(Game::renderer, sprite, nullptr, &destRect);
    if (isSelected)
    {
        blinkTimer = (blinkTimer + 1) % 50;
        int alpha = (blinkTimer < 25) ? 128 : 255;
        SDL_SetRenderDrawBlendMode(Game::renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(Game::renderer, 255, 255, 255, alpha);
        SDL_RenderDrawRect(Game::renderer, &destRect);
    }
}

void renderDeck(Card* deck[])
{
    int index = 0;
    for (int i = 0; i < TOTAL_CARDS; i++)
    {
        if (deck[i] == nullptr) continue;

        int w = deck[i]->getPos().w;
        int h = deck[i]->getPos().h;
        SDL_Rect destRect = {64 + index * (w + 16), SCREEN_HEIGHT - h - 64, w, h};

        deck[i]->render(destRect, i == selected);

        index++;
    }
}

void handleCardEvents(SDL_Event &event, Enemy* enemy, Card* deck[])
{
    static Uint32 lastKeyPress = 0;
    Uint32 currentTime = SDL_GetTicks();
    if ((event.type == SDL_KEYDOWN) && (currentTime - lastKeyPress > 100))
    {
        switch(event.key.keysym.sym)
        {
            case SDLK_LEFT: selected = (selected - 1 + validCards) % validCards; break;
            case SDLK_RIGHT: selected = (selected + 1) % validCards; break;
            case SDLK_RETURN: deck[selected]->useCard(selected, enemy); break;
            case SDLK_SPACE:
                if (selected < validCards - 1 && deck[selected]->canMerge(deck[selected + 1]))
                {
                    deck[selected]->mergeWith(deck[selected + 1], deck);
                }
        }
        lastKeyPress = currentTime;
    }
}

void Card::useCard(int cardIndex, Enemy* enemy)
{
    enemy->takeDamage(5);
}

bool Card::canMerge(Card* other)
{
    return (other != nullptr && type == other->type && star == other->star && star < 3);
}

void Card::mergeWith(Card* other, Card* deck[])
{
    star++;
    updateEffect();
    updateTexture();

    int removeIndex = selected + 1;
        /*for (int i = 0; i < TOTAL_CARDS; i++)
        {
            if (deck[i] = other)
            {
                removeIndex = i;
                break;
            }
        }*/
    delete deck[removeIndex];
    for (int i = removeIndex; i < TOTAL_CARDS - 1; i++)
    {
        deck[i] = deck[i + 1];
    }
    deck[TOTAL_CARDS - 1] = nullptr;
    validCards--;
}
