#include "Battle.h"
#include <iostream>

Turn current_turn = PLAYER_TURN;

int selecting = 0;
int selectOrder[TOTAL_CARDS] = {0};
int selected_count = 0;
int action_count = 0;

Uint32 cardplay_start = 0;
bool card_playing = false;
int play_order = 1;

void handlePlayerTurn(Player* player, Enemy* enemy, Card* deck[])
{
    if (current_turn != PLAYER_TURN) return;

    static Uint32 last_keypress = 0;
    Uint32 now = SDL_GetTicks();

    if ((Game::event.type == SDL_KEYDOWN) && (now - last_keypress > 100))
    {
        switch(Game::event.key.keysym.sym)
        {
            case SDLK_LEFT: selecting = (selecting - 1 + valid_cards) % valid_cards; break;
            case SDLK_RIGHT: selecting = (selecting + 1) % valid_cards; break;
            case SDLK_RETURN:
                if (selectOrder[selecting] > 0)
                {
                    int unselected = selectOrder[selecting];
                    selectOrder[selecting] = 0; // an enter lan 2 de bo select
                    for (int &i : selectOrder)
                    {
                        if (i > unselected) i--; // sua lai thu tu
                    }
                    selected_count--;
                    action_count--;
                }
                else if (selected_count < 3 && action_count < 3)
                {
                    selected_count++;
                    action_count++;
                    selectOrder[selecting] = selected_count;
                    std::cout << "Select card number " << selecting + 1 << std::endl;
                }
                break;
            case SDLK_m:
                if (selecting >= 0 && selecting < valid_cards - 1 && action_count < 3
                    && deck[selecting]->canMerge(deck[selecting + 1]))
                {
                    deck[selecting]->mergeWith(deck[selecting + 1], deck);
                        action_count++;
                }
                break;
            case SDLK_u:
                if (action_count == 3)
                {
                    play_order = 1;
                    card_playing = true;
                }
                break;
        }
        last_keypress = now;
    }
}

void updatePlayerTurn(Player* player, Enemy* enemy, Card* deck[])
{
    if (current_turn != PLAYER_TURN) return;

    if (card_playing && SDL_GetTicks() - cardplay_start > 2000)
    {
        for (int i = 0; i < valid_cards; i++)
        {
            if (selectOrder[i] == play_order && deck[i] != nullptr)
            {
                deck[i]->useCard(player, enemy);

                play_order++;
                cardplay_start = SDL_GetTicks();
                return; // ra ngoai de chay animation
            }
        }
        if (!enemy->isAlive() || !player->isAlive())
        {
            card_playing = false;
            return;
        }

        if (play_order > selected_count)
        {
            for (int i = valid_cards - 1; i >= 0; i--)
            {
                if (selectOrder[i] > 0 && deck[i] != nullptr) deck[i]->removeCard(i, deck);
            }
            // xoa the da su dung
            resetCardSelection();
            refillCardDeck(deck);
            action_count = 0;
            card_playing= false;
            current_turn = ENEMY_TURN;
        }
    }
}

void updateEnemyTurn(Player* player, Enemy* enemy, Card* deck[])
{
    if (current_turn != ENEMY_TURN) return;

    static Uint32 last_action = 0;
    Uint32 now = SDL_GetTicks();

    if (now - last_action > 1500)
    {
        player->takeDamage(enemy->attack);
        std::cout << "Enemy attacked!\n";
        SDL_Delay(1500);
        last_action = now;
        current_turn = PLAYER_TURN;
    }
}

void resetCardSelection()
{
    for (int &i : selectOrder) i = 0;
    selected_count = 0;
}

void refillCardDeck(Card* deck[])
{
    while (valid_cards < TOTAL_CARDS)
    {
        deck[valid_cards] = getRandomCard();
        valid_cards++;
    }
}
