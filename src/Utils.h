#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include <SDL.h>
#include <string>
#include <SDL_mixer.h>
#include "TileSet.h"
#include "Entity.h"
#include "Player.h"

SDL_Texture* loadTexture(const char* file);

bool checkEntity(const SDL_Rect &a, const SDL_Rect &b);

bool checkCollision(int w, int h, int x, int y);
bool atBorder(int x, int y);

bool availablePosition(int x, int y);

void renderText(const std::string text, SDL_Color color, int size, int x, int y);

void renderHP(Entity* entity, int x, int y);

extern Mix_Chunk* explosion_sfx;
extern Mix_Chunk* heal_sfx;
extern Mix_Chunk* click_sfx;
extern Mix_Music* adv_bgm;
extern Mix_Music* battle_bgm;

void loadMusic();

void cleanMusic();

#endif // UTILS_H_INCLUDED
