#ifndef BATTLE_H_INCLUDED
#define BATTLE_H_INCLUDED

#include "Entity.h"
#include "Player.h"
#include "Enemy.h"
#include "Card.h"

enum Turn {PLAYER_TURN, ENEMY_TURN};

extern Turn current_turn;
extern Card* deck[TOTAL_CARDS];

extern int selecting;
extern int selectOrder[TOTAL_CARDS];
extern bool card_playing;

void startBattle();

void endBattle();

void renderBattle(Entity* player, Entity* enemy);

void handlePlayerTurn(Player* player, Enemy* enemy, Card* deck[]);
void updatePlayerTurn(Player* player, Enemy* enemy, Card* deck[]);

void updateEnemyTurn(Player* player, Enemy* enemy, Card* deck[]);

void resetCardSelection();

void refillCardDeck(Card* deck[]);

#endif // BATTLE_H_INCLUDED
