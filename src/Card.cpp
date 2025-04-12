#include "Card.h"
#include "Game.h"
#include "Defs.h"
#include "Utils.h"
#include "Battle.h"
#include <SDL_image.h>
#include <iostream>

//-805306369 error

int valid_cards = TOTAL_CARDS;

const int CARD_WIDTH = 64;
const int CARD_HEIGHT = 96;

Card::Card(CardType t, int s) : type(t), star(s)
{
    updateCard();
}

Card::~Card()
{
    SDL_DestroyTexture(sprite);
}

void Card::updateCard()
{
    std::string file("");
    if (type == ATTACK)
    {
        if (star < 3) file = "assets/cards/attack_" + std::to_string(star) + ".png";
        else if (star == 3)
        {
            type = DEBUFF;
            file = "assets/cards/debuff.png";
        }
    }
    else if (type == HEAL)
    {
        if (star < 3) file = "assets/cards/heal_" + std::to_string(star) + ".png";
        else if (star == 3)
        {
            type = SHIELD;
            file = "assets/cards/shield.png";
        }
    }
    sprite = IMG_LoadTexture(Game::renderer, file.c_str());
    if (sprite == nullptr)
    {
        std::cout << "Failed to load card sprite!" << IMG_GetError() << std::endl;
    }
}

void Card::useCard(Entity* user, Entity* target)
{
    if (user == nullptr || target == nullptr)
    {
        std::cout << "NULL entity!\n";
        return;
    }
    if (type == ATTACK || type == DEBUFF)
    {
        target->takeDamage(user->attack * star * 1.5);
        user->changeState(State::ATTACK);
        std::cout << "Attack card used!\n";
    }
    else if (type == HEAL)
    {
        user->heal(user->attack / 2);
        user->changeState(State::HEAL);
        std::cout << "Heal card used!\n";
    }
    else if (type == SHIELD)
    {
        user->changeState(State::SHIELD);
        std::cout << "Shield card used!\n";
    }
}

void Card::removeCard(int index, Card* deck[])
{
    delete deck[index];

    for (int i = index; i < valid_cards - 1; i++)
    {
        deck[i] = deck[i + 1];
    }
    deck[valid_cards - 1] = nullptr;
    valid_cards--;
}

Card* getRandomCard()
{
    CardType type = (rand() % 2) ? ATTACK : HEAL;
    return new Card(type, 1);
}

bool Card::canMerge(Card* other)
{
    return (other != nullptr && type == other->type && star == other->star && star < 3);
}

void Card::mergeWith(Card* other, Card* deck[])
{
    star++;
    updateCard();

    removeCard(selecting + 1, deck);
}

void Card::render(SDL_Rect &destRect, int index, bool isSelecting)
{
    if (selectOrder[index] > 0) SDL_SetTextureColorMod(sprite, 128, 128, 128);
    else SDL_SetTextureColorMod(sprite, 255, 255, 255);

    SDL_RenderCopy(Game::renderer, sprite, nullptr, &destRect);

    if (selectOrder[index] > 0 || current_turn != PLAYER_TURN)
    {
        SDL_Color color = {234, 123, 012, 255};
        int x = destRect.x + CARD_WIDTH;
        int y = destRect.y + CARD_HEIGHT;
        renderText(std::to_string(selectOrder[index]), color, 40, x, y);
    }
    if (isSelecting)
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
    for (int i = 0; i < valid_cards; i++)
    {
        if (deck[i] == nullptr) continue;

        int w = CARD_WIDTH * 2;
        int h = CARD_HEIGHT * 2;
        SDL_Rect destRect = {100 + index * (w + 30), SCREEN_HEIGHT - h - 64, w, h};

        deck[i]->render(destRect, i, i == selecting);

        index++;
    }
}
