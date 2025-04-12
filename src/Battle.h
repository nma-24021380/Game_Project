#ifndef BATTLE_H_INCLUDED
#define BATTLE_H_INCLUDED

#include "Player.h"
#include "Enemy.h"
#include "Card.h"

enum Turn {PLAYER_TURN, ENEMY_TURN};

extern Turn current_turn;
extern int selecting;
extern int selectOrder[TOTAL_CARDS];

void handlePlayerTurn(Player* player, Enemy* enemy, Card* deck[]);
void updatePlayerTurn(Player* player, Enemy* enemy, Card* deck[]);

void updateEnemyTurn(Player* player, Enemy* enemy, Card* deck[]);

void resetCardSelection();

void refillCardDeck(Card* deck[]);

#endif // BATTLE_H_INCLUDED
