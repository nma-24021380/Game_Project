#include "Card.h"
#include "Game.h"
#include "Defs.h"
#include "Utils.h"
#include "Battle.h"
#include <iostream>

int valid_cards = TOTAL_CARDS;

Card* deck[TOTAL_CARDS] = {nullptr};

Card::Card(CardType t, int s) : type(t), star(s)
{
    updateCard();
}

Card::Card(const Card& other)
{
    type = other.type;
    star = other.star;
    sprite = nullptr;
    updateCard();
}

Card::~Card()
{
    if (sprite)
    {
        SDL_DestroyTexture(sprite);
        sprite = nullptr;
    }
}

void Card::updateCard()
{
    std::string file("");
    if (type == ATTACK)
    {
        if (star < 3) file = "assets/cards/attack_" + std::to_string(star) + ".png";
        else if (star == 3)
        {
            type = STUN;
            file = "assets/cards/stun.png";
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
    sprite = loadTexture(file.c_str());
}

void Card::useCard(Entity* user, Entity* target)
{
    if (type == ATTACK)
    {
        target->takeDamage(user->attack * star * 1.5);

        user->changeState(State::ATTACK);
        std::cout << "Attack card used!\n";
    }
    else if (type == STUN)
    {
        target->takeDamage(user->attack * star * 1.5);
        target->applyStun();

        user->changeState(State::ATTACK);
        std::cout << "Stun card used!\n";
    }
    else if (type == HEAL)
    {
        user->heal(user->attack * star);

        user->changeState(State::HEAL);
        std::cout << "Heal card used!\n";
    }
    else if (type == SHIELD)
    {
        user->applyShield();
        std::cout << "Shield card used!\n";
    }
}

void Card::removeCard(int index)
{
    delete deck[index];

    for (int i = index; i < valid_cards - 1; ++i)
    {
        deck[i] = deck[i + 1];
    }
    deck[valid_cards - 1] = nullptr;
    valid_cards--;
}

Card* getRandomCard()
{
    CardType type = ((rand() % 3) < 2) ? ATTACK : HEAL;
    return new Card(type, 1);
}

bool Card::canMerge(Card* other)
{
    return (other != nullptr && type == other->type && star == other->star && star < 3);
}

void Card::mergeWith(Card* other)
{
    ++star;
    updateCard();

    removeCard(selecting + 1);
}

void Card::render(SDL_Rect &destRect, int index, bool is_selecting)
{
    if (selectOrder[index] > 0 || card_playing) SDL_SetTextureColorMod(sprite, 128, 128, 128);
    else SDL_SetTextureColorMod(sprite, 255, 255, 255);

    if (sprite) SDL_RenderCopy(Game::renderer, sprite, nullptr, &destRect);

    if (selectOrder[index] > 0)
    {
        SDL_Color color = {234, 123, 012, 255};
        int x = destRect.x + CARD_WIDTH - 20;
        int y = destRect.y + CARD_HEIGHT - 20;
        renderText(std::to_string(selectOrder[index]), color, 75, x, y);
    }
    if (is_selecting)
    {
        static int blink_timer = 0;
        blink_timer = (blink_timer + 1) % 50;
        int alpha = (blink_timer < 25) ? 128 : 255;
        SDL_SetRenderDrawBlendMode(Game::renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(Game::renderer, 255, 255, 255, alpha);

        SDL_RenderDrawRect(Game::renderer, &destRect);
    }
}

void renderDeck()
{
    int index = 0;
    for (int i = 0; i < valid_cards; ++i)
    {
        if (deck[i] == nullptr) continue;

        int w = CARD_WIDTH * 1.5;
        int h = CARD_HEIGHT * 1.5;
        SDL_Rect destRect = {50 + index * (w + 30), SCREEN_HEIGHT - h - 50, w, h};

        deck[i]->render(destRect, i, i == selecting);

        ++index;
    }
}

void refillCardDeck()
{
    while (valid_cards < TOTAL_CARDS)
    {
        deck[valid_cards] = getRandomCard();
        ++valid_cards;
    }
}

void mergeCards(int index)
{
    if (selecting >= 0 && selecting < valid_cards - 1
        && selectOrder[selecting] == 0 && selectOrder[selecting + 1] == 0
        && deck[selecting]->canMerge(deck[selecting + 1]))
    {
        deck[index]->mergeWith(deck[index + 1]);

        for (int i = index + 1; i < valid_cards - 1; ++i)
        {
            selectOrder[i] = selectOrder[i + 1];
        }
    }
}
