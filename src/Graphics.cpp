#include "Graphics.h"
#include "Game.h"
#include <SDL_image.h>

Animation::Animation()
: spriteSheet(nullptr), frameW(0), frameH(0), totalFrames(0), frameSpeed(0)
{}

Animation::Animation(const char* file, int w, int h, int total, int speed)
: frameW(w), frameH(h), totalFrames(total), frameSpeed(speed)
{
    spriteSheet = IMG_LoadTexture(Game::renderer, file);
    srcRect = {0, 0, frameW, frameH};
}

void Animation::start()
{
    playing = true;
    currentFrame = 0;
}

void Animation::update()
{
    if (!playing) return;
    static Uint32 lastFrameTime = 0;
    Uint32 currentTime = SDL_GetTicks();
    if (currentTime - lastFrameTime > frameSpeed)
    {
        currentFrame++;
        if (currentFrame >= totalFrames)
        {
            currentFrame = 0;
            playing = false;
        }
        lastFrameTime = currentTime;
    }
}

void Animation::render(int x, int y, int row)
{
    if (!playing) return;
    srcRect.x = currentFrame * frameW;
    srcRect.y = row * frameH;
    SDL_Rect destRect = {x, y, frameW * 2, frameH * 2};
    SDL_RenderCopy(Game::renderer, spriteSheet, &srcRect, &destRect);
}

bool Animation::isPlaying()
{
    return playing;
}
