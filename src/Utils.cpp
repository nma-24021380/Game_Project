#include "Utils.h"

bool checkCollision(const SDL_Rect &a, const SDL_Rect &b)
{
    return SDL_HasIntersection(&a, &b);
}
