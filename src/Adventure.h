#ifndef ADVENTURE_H_INCLUDED
#define ADVENTURE_H_INCLUDED

#include <SDL.h>
#include <SDL_mixer.h>

void startAdventure();
void cleanAdventure();

void handleAdventure();

bool checkWinning();

void renderAdventure();
void renderAdvGuide();

#endif // ADVENTURE_H_INCLUDED
