#ifndef BATTLE_H_INCLUDED
#define BATTLE_H_INCLUDED

#include "Entity.h"
#include "Player.h"
#include "Enemy.h"
#include "Card.h"

enum Turn {PLAYER_TURN, ENEMY_TURN};

extern Turn current_turn;

extern int selecting;
extern int selectOrder[TOTAL_CARDS];
extern bool card_playing;

void startBattle(Enemy* enemy);
void endBattle();
void cleanBattle();

void handleBattle();

void renderBattle();
void renderInfo(Entity* entity, int x, int y);
void renderConfirmMenu();

void handlePlayerTurn();

void updatePlayerTurn();
void updateCardPlay();
void updateEnemyTurn();

void unselectCard(int index);
void selectCard(int index);
void startCardPlay();
void resetOriginalDeck();

void resetCardSelection();

#endif // BATTLE_H_INCLUDED
