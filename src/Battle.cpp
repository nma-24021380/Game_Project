#include "Battle.h"
#include "Utils.h"
#include "Defs.h"
#include "Game.h"
#include "SceneManager.h"
#include <algorithm>
#include <iostream>
#include <SDL_mixer.h>

Turn current_turn = PLAYER_TURN;
int turn_count = 1;
bool battle_ready = false;

Card* backupDeck[TOTAL_CARDS] = {nullptr};
bool backed_up = false;

SDL_Texture* background = nullptr;
SDL_Texture* button = nullptr;

SDL_Texture* battle_guide = nullptr;
bool battle_guide_on = false;

int selecting = 0;                  // vi tri card hien tai
int selectOrder[TOTAL_CARDS] = {0}; // thu tu chon card
int selected_count = 0;             // so luong card da chon
int action_count = 0;               // so luot hanh dong hien tai

bool card_playing = false;
int play_order = 1;

std::set<std::pair<int, int>> defeated_maps;
int enemy_card_count = 0;

bool in_confirm_menu = false;

void startBattle(Enemy* enemy)
{
    current_enemy = enemy;
    current_turn = PLAYER_TURN;
    std::cout << "Battle started!\n";

    if (!background) background = loadTexture("assets/battle_bg.png");

    if (!button) button = loadTexture("assets/button.png");

    if (!battle_guide) battle_guide = loadTexture("assets/battle_guide.png");

    if (Mix_PlayingMusic()) Mix_HaltMusic();
    Mix_FadeOutMusic(1000);
    Mix_FadeInMusic(battle_bgm, -1, 1000);

    // tao deck ngau nhien
    for (int i = 0; i < TOTAL_CARDS; ++i)
    {
        deck[i] = getRandomCard();
        if (!deck[i]) return;
    }

    resetCardSelection();

    action_count = 0;
    enemy_card_count = 0;
    turn_count = 1;

    card_playing = false;
    play_order = 1;
    backed_up = false;
    in_confirm_menu = false;

    battle_ready = true;
}

void endBattle()
{
    for (int i = 0; i < TOTAL_CARDS; ++i)
    {
        if (deck[i])
        {
            delete deck[i];
            deck[i] = nullptr;
        }
        if (backupDeck[i])
        {
            delete backupDeck[i];
            backupDeck[i] = nullptr;
        }
    }

    if (current_enemy)
    {
        if (!current_enemy->isAlive())
        {
            auto it = std::find(enemies.begin(), enemies.end(), current_enemy);
            if (it != enemies.end())
            {
                delete *it;
                enemies.erase(it);
            }
        }
        current_enemy = nullptr;
    }

    battle_ready = false;

    changeScene(SceneState::ADVENTURE);

    if (player)
    {
        player->changeState(State::IDLE);
        player->clearStatusEffects();
    }
}

void cleanBattle()
{
    endBattle();

    if (background)
    {
        SDL_DestroyTexture(background);
        background = nullptr;
    }

    if (button)
    {
        SDL_DestroyTexture(button);
        button = nullptr;
    }

    if (battle_guide)
    {
        SDL_DestroyTexture(battle_guide);
        battle_guide = nullptr;
    }

    for (int i = 0; i < TOTAL_CARDS; ++i)
    {
        if (deck[i])
        {
            delete deck[i];
            deck[i] = nullptr;
        }
        if (backupDeck[i])
        {
            delete backupDeck[i];
            backupDeck[i] = nullptr;
        }
    }
}

void handleBattle()
{
    if (Game::event.type == SDL_KEYDOWN && Game::event.key.keysym.sym == SDLK_g)
    {
        battle_guide_on = !battle_guide_on;

        if (click_sfx) Mix_PlayChannel(-1, click_sfx, 0);
    }
}

void renderBattle()
{
    if (!battle_ready) return;
    if (background) SDL_RenderCopy(Game::renderer, background, nullptr, nullptr);

    if (player) player->render();
    if (current_enemy) current_enemy->render();

    renderInfo(player, PLAYER_BATTLE_X, PLAYER_BATTLE_Y - 20);
    renderInfo(current_enemy, ENEMY_BATTLE_X + 75, ENEMY_BATTLE_Y - 20);

    if (deck[0] != nullptr) renderDeck();

    if (action_count == 3) renderConfirmMenu();

    SDL_Color color = {0, 0, 0, 255};   // render guide
    renderText("G", color, 50, 15, 15);

    if (battle_guide && battle_guide_on)
    {
        SDL_Rect destRect = {(SCREEN_WIDTH - 824) / 2, (SCREEN_HEIGHT - 675) / 2, 824, 675};
        SDL_RenderCopy(Game::renderer, battle_guide, NULL, &destRect);
    }
}

void renderInfo(Entity* entity, int x, int y)
{
    SDL_Color color = {255, 255, 255, 255};
    renderHP(entity, x, y);
    renderText(entity->name, color, 16, x, y - 20);
}

void renderConfirmMenu()
{
    SDL_Rect srcRect = {0, 0, 140, 40};
    SDL_Rect destRect = {SCREEN_WIDTH - 200, SCREEN_HEIGHT - 200, srcRect.w, srcRect.h};

    static int blink_timer = 0;
    blink_timer = (blink_timer + 1) % 100;
    int alpha = (blink_timer < 25) ? 128 : 255;

    // buttons
    for (int i = 0; i <= 1; ++i)
    {
        srcRect.x = i * srcRect.w;
        destRect.y = SCREEN_HEIGHT - 200 + i * (srcRect.h + 25);
        SDL_RenderCopy(Game::renderer, button, &srcRect, &destRect);
    }
    // blink effect
    SDL_SetRenderDrawBlendMode(Game::renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(Game::renderer, 255, 255, 255, alpha);
    destRect.y = SCREEN_HEIGHT - 200 + (selecting + 2) * (srcRect.h + 25);
    SDL_RenderDrawRect(Game::renderer, &destRect);
}

void handlePlayerTurn()
{
    if (!battle_ready || current_turn != PLAYER_TURN) return;

    static Uint32 last_keypress = 0;

    if ((Game::event.type == SDL_KEYDOWN) && (SDL_GetTicks() - last_keypress > 100))
    {
        switch(Game::event.key.keysym.sym)
        {
        case SDLK_UP:
            if (in_confirm_menu) selecting = -2;
            break;
        case SDLK_DOWN:
            if (in_confirm_menu) selecting = -1;
            break;
        case SDLK_LEFT:
            if (!in_confirm_menu) selecting = (selecting - 1 + valid_cards) % valid_cards;
            break;
        case SDLK_RIGHT:
            if (!in_confirm_menu) selecting = (selecting + 1) % valid_cards;
            break;
        case SDLK_RETURN:
            if (!in_confirm_menu)
            {
                if (selectOrder[selecting] > 0 && selected_count > 0) unselectCard(selecting);
                else if (selected_count < 3) selectCard(selecting);
            }
            else // confirm menu
            {
                if (selecting == -2) startCardPlay();
                else if (selecting == -1) resetOriginalDeck();
            }
            break;
        case SDLK_m:
            mergeCards(selecting);
            ++action_count;
            break;
        }

        if (click_sfx) Mix_PlayChannel(-1, click_sfx, 0);

        last_keypress = SDL_GetTicks();
    }
}

void unselectCard(int index)
{
    int unselected = selectOrder[index];
    selectOrder[index] = 0;     // bo chon card
    for (int &i : selectOrder)  // update thu tu chon card
    {
        if (i > unselected) i--;
    }
    selected_count--;
    action_count--;
}

void selectCard(int index)
{
    ++selected_count;
    selectOrder[index] = selected_count;
    ++action_count;
}

void startCardPlay()
{
    play_order = 1;
    card_playing = true;
}

void resetOriginalDeck()
{
    resetCardSelection();
    valid_cards = TOTAL_CARDS;
    action_count = 0;
    for (int i = 0; i < TOTAL_CARDS; ++i)
    {
        if (deck[i])
        {
            delete deck[i];
            deck[i] = nullptr;
        }
    }

    for (int i = 0; i < TOTAL_CARDS; ++i)   // quay lai deck ban dau
    {
        if (backupDeck[i])
        {
            deck[i] = new Card(*backupDeck[i]);
            if (!deck[i]) return;
        }
    }

    selecting = 0;
    in_confirm_menu = false;
}

void updatePlayerTurn()
{
    if (!battle_ready || current_turn != PLAYER_TURN) return;

    if (player->isStunned())
    {
        std::cout << "Player stunned\n";
        player->clearStatusEffects();
        current_turn = ENEMY_TURN;
        return;
    }

    // backup deck ban dau
    if (action_count == 0 && !backed_up)
    {
        for (int i = 0; i < TOTAL_CARDS; ++i)
        {
            if (backupDeck[i]) delete backupDeck[i];
            backupDeck[i] = new Card(*deck[i]);
        }
        backed_up = true;
    }

    if (action_count == 3 && !in_confirm_menu) // update confirm menu
    {
        in_confirm_menu = true;
        selecting = -2;
    }
    else if (action_count < 3)
    {
        in_confirm_menu = false;
    }

    if (card_playing)
    {
        updateCardPlay();
    }

    if (!current_enemy->isAlive())
    {
        std::cout << "Enemy defeated!\n";
        endBattle();
        changeScene(SceneState::ADVENTURE);
        return;
    }
}

void updateCardPlay()
{
    static Uint32 cardplay_start = 0;

    if (SDL_GetTicks() - cardplay_start < 1500) return;

    for (int i = 0; i < valid_cards; ++i)
    {
        if (selectOrder[i] == play_order && deck[i] != nullptr)
        {
            deck[i]->useCard(player, current_enemy);

            ++play_order;
            cardplay_start = SDL_GetTicks();
            return; // quay lai chay animation
        }
    }
    if (!current_enemy->isAlive() || !player->isAlive())
    {
        card_playing = false;
        return;
    }
    if (play_order > selected_count)
    {
        for (int i = valid_cards - 1; i >= 0; --i)
        {
            if (selectOrder[i] > 0 && deck[i] != nullptr) deck[i]->removeCard(i);
        }
        // xoa card da su dung
        resetCardSelection();
        refillCardDeck();
        action_count = 0;
        card_playing = false;
        backed_up = false;

        ++turn_count;
        current_turn = ENEMY_TURN;
        std::cout << "Enemy turn!\n";
    }
}

void updateEnemyTurn()
{
    if (!battle_ready || current_turn != ENEMY_TURN) return;

    if (current_enemy->isStunned())
    {
        std::cout << "Enemy stunned\n";
        current_enemy->clearStatusEffects();
        enemy_card_count = 0;

        current_turn = PLAYER_TURN;
        std::cout << "Player turn!\n";
        return;
    }

    if (enemy_card_count == 0)
    {
        current_enemy->refillEnemyDeck();
        if (turn_count > 5 && turn_count % 3 == 0) current_enemy->addAttack();
    }

    static Uint32 last_action = 0;
    if (SDL_GetTicks() - last_action > 1500 && enemy_card_count < 2)
    {
        current_enemy->useCard(enemy_card_count);
        //std::cout << "Enemy used card " << enemy_card_count + 1 << "!\n";
        ++enemy_card_count;
        last_action = SDL_GetTicks();

        if (enemy_card_count >= 2)
        {
            if (player->isAlive())
            {
                enemy_card_count = 0;
                ++turn_count;
                std::cout << current_enemy->attack << std::endl;
                current_turn = PLAYER_TURN;
                std::cout << "Player turn!\n";
            }
        }
    }

    if (!current_enemy->isAlive())
    {
        std::cout << "Enemy defeated!\n";
        endBattle();
        changeScene(SceneState::ADVENTURE);
        return;
    }
}

void resetCardSelection()
{
    for (int &i : selectOrder) i = 0;
    selected_count = 0;
    selecting = 0;
}
