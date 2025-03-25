#include "TileSet.h"
#include <SDL_image.h>
#include <iostream>
#include "Defs.h"

TileSet::TileSet(SDL_Renderer* renderer, const char* file, int size)
{
    tileSize = size;
    tilesetTex = IMG_LoadTexture(renderer, file);
    if (tilesetTex == nullptr)
    {
        std::cout << "Failed to render tile set! Error: " << IMG_GetError() << std::endl;
    }
}

void TileSet::render(SDL_Renderer* renderer, const std::vector<std::vector<int>> &mapData)
{
    for (int y = 0; y < (int)mapData.size(); y++)
    {
        for (int x = 0; x < (int)mapData[y].size(); x++)
        {
            int tileID = mapData[y][x];

            SDL_Rect srcRect = {tileID * tileSize, 0, tileSize, tileSize};
            SDL_Rect destRect = {x * tileSize, y * tileSize, tileSize, tileSize};

            SDL_RenderCopy(renderer, tilesetTex, &srcRect, &destRect);
        }
    }
}
