#ifndef TILESET_H_INCLUDED
#define TILESET_H_INCLUDED

#include <SDL.h>
#include <vector>
#include "Game.h"

class TileSet
{
public:
    TileSet(const char* sprite_png);
    ~TileSet();

    int getTileID(int x, int y);
    int getMapX();
    int getMapY();

    void loadTileMap();
    void changeTileMap(int x, int y);
    void render();

private:
    SDL_Texture* sprite;
    std::vector<std::vector<int>> tileMap;
    int mapX, mapY;
};

#endif // TILESET_H_INCLUDED
