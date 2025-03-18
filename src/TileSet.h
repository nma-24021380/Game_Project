#ifndef TILESET_H_INCLUDED
#define TILESET_H_INCLUDED

#include <SDL.h>
#include <vector>

class TileSet
{
public:
    TileSet(SDL_Renderer* renderer, const char* file, int size);
    void render(SDL_Renderer* renderer, const std::vector<std::vector<int>> &mapData);


private:
    SDL_Texture* tilesetTex;
    int tileSize;
};

#endif // TILESET_H_INCLUDED
