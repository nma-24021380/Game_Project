#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include <SDL.h>
#include <string>
#include "TileSet.h"

bool checkEnemy(const SDL_Rect &a, const SDL_Rect &b);

bool checkCollision(TileSet* tileMap, int x, int y);

bool atBorder(int x, int y);

void renderText(const std::string text, SDL_Color color, int size, int x, int y);

#endif // UTILS_H_INCLUDED
