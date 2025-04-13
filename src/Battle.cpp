#include "Battle.h"
#include "Utils.h"
#include "Defs.h"
#include "Game.h"
#include <iostream>

Turn current_turn = PLAYER_TURN;
Card* deck[TOTAL_CARDS] = {nullptr};
Card* backupDeck[TOTAL_CARDS] = {nullptr};
bool backed_up = false;

SDL_Texture* background = nullptr;
SDL_Texture* button = nullptr;

int selecting = 0;
int selectOrder[TOTAL_CARDS] = {0};
int selected_count = 0;
int action_count = 0;

Uint32 cardplay_start = 0;
bool card_playing = false;
int play_order = 1;

bool in_confirm_menu = false;

void startBattle()
{
    if (!background) background = loadTexture("assets/battle_bg.png");
    if (!button) button = loadTexture("assets/button.png");

    for (int i = 0; i < TOTAL_CARDS; i++)
    {
        deck[i] = getRandomCard();
        if (deck[i] == nullptr) std::cout << "Failed to initialize card!";
    }
}

void endBattle()
{
    SDL_DestroyTexture(background);
    SDL_DestroyTexture(button);

    for (int i = 0; i < TOTAL_CARDS; i++)
    {
        delete deck[i]; deck[i] = nullptr;
        delete backupDeck[i]; backupDeck[i] = nullptr;
    }
}

void renderBattle(Entity* player, Entity* enemy)
{
    if (background) SDL_RenderCopy(Game::renderer, background, nullptr, nullptr);

    SDL_Color color = {255, 255, 255, 255};

    renderHP(player, 250, SCREEN_HEIGHT / 2 - 20);
    renderText(player->name, color, 16, 250, SCREEN_HEIGHT / 2 - 40);

    renderHP(enemy, SCREEN_WIDTH - 350, SCREEN_HEIGHT / 2 - 20);
    renderText(enemy->name, color, 16, SCREEN_WIDTH - 350, SCREEN_HEIGHT / 2 - 40);

    if (action_count == 3)
    {
        SDL_Rect srcRect = {0, 0, 84, 84};
        SDL_Rect destRect = {SCREEN_WIDTH - 200, SCREEN_HEIGHT - 200, srcRect.w, srcRect.h};

        static int blink_timer = 0;
        blink_timer = (blink_timer + 1) % 50;

        for (int i = 0; i <= 1; i++)
        {
            srcRect.x = i * srcRect.w;
            destRect.y = SCREEN_HEIGHT - 250 + i * (srcRect.w + 25);
            SDL_RenderCopy(Game::renderer, button, &srcRect, &destRect);
        }

        int alpha = (blink_timer < 25) ? 128 : 255;
        SDL_SetRenderDrawBlendMode(Game::renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(Game::renderer, 255, 255, 255, alpha);

        destRect.y = SCREEN_HEIGHT - 250 + (selecting + 2) * (srcRect.w + 25);
        SDL_RenderDrawRect(Game::renderer, &destRect);
        // ve vien nhap nhay
    }
}

void handlePlayerTurn(Player* player, Enemy* enemy, Card* deck[])
{
    if (current_turn != PLAYER_TURN) return;

    static Uint32 last_keypress = 0;
    Uint32 now = SDL_GetTicks();

    if (action_count == 0 && !backed_up)
    {
        for (int i = 0; i < TOTAL_CARDS; i++)
        {
            if (backupDeck[i]) delete backupDeck[i];
            backupDeck[i] = new Card(*deck[i]);
        }
        backed_up = true;
    }

    if ((Game::event.type == SDL_KEYDOWN) && (now - last_keypress > 100))
    {
        switch(Game::event.key.keysym.sym)
        {
        case SDLK_UP: if (in_confirm_menu) selecting = -2; break;
        case SDLK_DOWN: if (in_confirm_menu) selecting = -1; break;

        case SDLK_LEFT: if (!in_confirm_menu) selecting = (selecting - 1 + valid_cards) % valid_cards; break;
        case SDLK_RIGHT: if (!in_confirm_menu) selecting = (selecting + 1) % valid_cards; break;

        case SDLK_RETURN:
            if (!in_confirm_menu)
            {
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
                else if (selected_count < 3)
                {
                    selected_count++;
                    action_count++;
                    selectOrder[selecting] = selected_count;
                    std::cout << "Select card number " << selecting + 1 << std::endl;
                }
            }
            else if (in_confirm_menu)
            {
                if (selecting == -2)
                {
                    play_order = 1;
                    card_playing = true;
                }
                else if (selecting == -1)
                {
                    resetCardSelection();
                    action_count = 0;
                    valid_cards = 0;
                    for (int i = 0; i < TOTAL_CARDS; i++)
                    {
                        if (deck[i]) { delete deck[i]; deck[i] = nullptr; }
                        if (backupDeck[i]) {
                        deck[valid_cards++] = new Card(*backupDeck[i]);
                        }
                    }
                }
            }
            break;
        case SDLK_m:
            if (selecting >= 0 && selecting < valid_cards - 1 && !in_confirm_menu
                && deck[selecting]->canMerge(deck[selecting + 1]))
            {
                deck[selecting]->mergeWith(deck[selecting + 1], deck);
                action_count++;
            }
            break;
        }
        last_keypress = now;
    }
}

void updatePlayerTurn(Player* player, Enemy* enemy, Card* deck[])
{
    if (current_turn != PLAYER_TURN) return;

    if (action_count == 3 && !in_confirm_menu)
    {
        in_confirm_menu = true;
        selecting = -2;
    }
    else if (action_count < 3) in_confirm_menu = false;

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
            card_playing = false;
            backed_up = false;

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
    selecting = 0;
}

void refillCardDeck(Card* deck[])
{
    while (valid_cards < TOTAL_CARDS)
    {
        deck[valid_cards] = getRandomCard();
        valid_cards++;
    }
}
