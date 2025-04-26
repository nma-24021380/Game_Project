#include "TileSet.h"
#include "Defs.h"
#include "Utils.h"
#include <iostream>
#include <fstream>
#include <string>

TileSet* tile_map = nullptr;

TileSet::TileSet(const char* sprite_png)
{
    sprite = loadTexture(sprite_png);
    map_x = 0;
    map_y = 0; // mac dinh la map (0, 0) -> tilemap_0_0
    loadTileMap();
}

TileSet::~TileSet()
{
    SDL_DestroyTexture(sprite);
}

int TileSet::getTileID(int x, int y)
{
    return tile_map[y][x];
}

int TileSet::getMapX() { return map_x; }

int TileSet::getMapY() { return map_y; }

void TileSet::loadTileMap()
{
    std::string tilemap_txt = "assets/map/tilemap_" + std::to_string(map_x) + "_" + std::to_string(map_y) + ".txt";

    std::ifstream mapFile(tilemap_txt.c_str());
    if (!mapFile)
    {
        std::cout << "Failed to open map file! " << tilemap_txt << std::endl;
    }

    tile_map.clear();
    std::vector<int> row;

    int tileID;
    while (mapFile >> tileID)
    {
        row.push_back(tileID);
        if (row.size() == MAP_WIDTH)
        {
            tile_map.push_back(row);
            row.clear();
        }
    }
    mapFile.close();
}

void TileSet::changeTileMap(int x, int y)
{
    map_x = x;
    map_y = y;
    loadTileMap();
}

void TileSet::changeMap(int &player_x, int &player_y, int direction)
{
    switch(direction)
    {
        case 0:
            player_y = SCREEN_HEIGHT - PLAYER_HEIGHT - 1;
            if (map_y > 0)
            {
                changeTileMap(map_x, map_y - 1);
            }
            break;
        case 1:
            player_y = 1;
            changeTileMap(map_x, map_y + 1);
            break;
        case 2:
            player_x = SCREEN_WIDTH - PLAYER_WIDTH - 1;
            if (map_x > 0)
            {
                changeTileMap(map_x - 1, map_y);
            }
            break;
        case 3:
            player_x = 1;
            changeTileMap(map_x + 1, map_y);
            break;
    }
}

void TileSet::render()
{
    for (int y = 0; y < MAP_HEIGHT; ++y)
    {
        for (int x = 0; x < MAP_WIDTH; ++x)
        {
            int tileID = tile_map[y][x];

            SDL_Rect srcRect = {tileID * TILE_SIZE, 0, TILE_SIZE, TILE_SIZE};
            SDL_Rect destRect = {x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE};

            SDL_RenderCopy(Game::renderer, sprite, &srcRect, &destRect);
        }
    }
}
