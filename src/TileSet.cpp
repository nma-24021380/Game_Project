#include "TileSet.h"
#include "Defs.h"
#include <SDL_image.h>
#include <iostream>
#include <fstream>
#include <string>

TileSet::TileSet(const char* sprite_png)
{
    sprite = IMG_LoadTexture(Game::renderer, sprite_png);
    if (sprite == nullptr)
    {
        std::cout << "Failed to load tile map! Error: " << IMG_GetError() << std::endl;
    }
    mapX = 0;
    mapY = 0; // mac dinh la map (0, 0) -> tilemap_0_0
    loadTileMap();
}

TileSet::~TileSet()
{
    SDL_DestroyTexture(sprite);
}

int TileSet::getTileID(int x, int y)
{
    return tileMap[y][x];
}

int TileSet::getMapX()
{
    return mapX;
}

int TileSet::getMapY()
{
    return mapY;
}

void TileSet::loadTileMap()
{
    std::string tilemap_txt = "assets/map/tilemap_" + std::to_string(mapX) + "_" + std::to_string(mapY) + ".txt";

    std::ifstream mapFile(tilemap_txt.c_str());
    if (!mapFile)
    {
        std::cout << "Failed to open map file! " << tilemap_txt << std::endl;
    }

    tileMap.clear();
    std::vector<int> row;

    int tileID;
    while (mapFile >> tileID)
    {
        row.push_back(tileID);
        if (row.size() == MAP_WIDTH)
        {
            tileMap.push_back(row);
            row.clear();
        }
    }
    mapFile.close();
}

void TileSet::changeTileMap(int x, int y)
{
    mapX = x;
    mapY = y;
    loadTileMap();
}

void TileSet::render()
{
    for (int y = 0; y < MAP_HEIGHT; y++)
    {
        for (int x = 0; x < MAP_WIDTH; x++)
        {
            int tileID = tileMap[y][x];

            SDL_Rect srcRect = {tileID * TILE_SIZE, 0, TILE_SIZE, TILE_SIZE};
            SDL_Rect destRect = {x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE};

            SDL_RenderCopy(Game::renderer, sprite, &srcRect, &destRect);
        }
    }
}
