#ifndef DEFS_H_INCLUDED
#define DEFS_H_INCLUDED

#include <vector>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 640;

const int TILE_SIZE = 128;

const int PLAYER_SPEED = 20;

const std::vector<std::vector<int>> grassMapData =
{
    {1, 0, 0, 0, 0},
    {0, 0, 1, 0, 1},
    {0, 0, 1, 0, 0},
    {0, 1, 0, 0, 0},
    {0, 1, 0, 1, 0},
    {0, 1, 0, 0, 0}
};

#endif // DEFS_H_INCLUDED
