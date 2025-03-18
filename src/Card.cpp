#include "Card.h"
#include <SDL_image.h>
#include <iostream>
#include "Game.h"
#include "Defs.h"

Card::Card(const char* file)
{
    cardPos.w = 64;
    cardPos.h = 96;

    cardSprite = IMG_LoadTexture(Game::renderer, file);
    if (cardSprite == NULL)
    {
        std::cout << "Failed to load card sprite!" << IMG_GetError() << std::endl;
    }
}

void Card::render()
{
    for (int i = 0; i < numCards; i++)
    {
        SDL_Rect desRect = {64 + i * (cardPos.w + 32), SCREEN_HEIGHT - cardPos.h - 64, cardPos.w, cardPos.h};
        if (i == selectedCard)
        {
            SDL_SetRenderDrawColor(Game::renderer, 255, 255, 255, 255);
            SDL_RenderFillRect(Game::renderer, &desRect);
            SDL_SetRenderDrawColor(Game::renderer, 0, 0, 0, 0);
        }
        else
        {
            SDL_RenderCopy(Game::renderer, cardSprite, NULL, &desRect);
        }
    }
}

void Card::cardSelection(SDL_Event &event, Enemy* enemy)
{
    Uint32 currentTime = SDL_GetTicks();
    if ((event.type == SDL_KEYDOWN) && (currentTime - lastKeyPress > 150))
    {
        switch(event.key.keysym.sym)
        {
            case SDLK_LEFT: selectedCard = (selectedCard - 1 + numCards) % numCards; break;
            case SDLK_RIGHT: selectedCard = (selectedCard + 1) % numCards; break;
            case SDLK_RETURN: useCard(selectedCard, enemy); break;
        }
        lastKeyPress = currentTime;
    }
}

void Card::useCard(int cardNum, Enemy* enemy)
{
    enemy->takeDamage(5);
    std::cout << "Use card number " << cardNum << std::endl;
    std::cout << "Enemy HP: " << enemy->health << std::endl;
}
