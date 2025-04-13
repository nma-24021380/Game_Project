#include "Graphics.h"
#include "Game.h"
#include "Utils.h"

Animation::Animation()
: spritesheet(nullptr), frame_w(0), frame_h(0), total_frames(0), frame_speed(0)
{}

Animation::Animation(const char* file, int w, int h, int total, int speed)
: frame_w(w), frame_h(h), total_frames(total), frame_speed(speed)
{
    spritesheet = loadTexture(file);
    srcRect = {0, 0, frame_w, frame_h};
}

void Animation::start()
{
    playing = true;
    current_frame = 0;
}

void Animation::update(bool loop)
{
    if (!playing) return;
    static Uint32 lastFrameTime = 0;
    Uint32 currentTime = SDL_GetTicks();

    if (currentTime - lastFrameTime > frame_speed)
    {
        if (loop)
        {
            current_frame = (current_frame + 1) % total_frames;
        }
        else
        {
            current_frame++;
            if (current_frame >= total_frames)
            {
                current_frame = 0;
                playing = false;
            }
        }
        lastFrameTime = currentTime;
    }
}

void Animation::render(int x, int y, int row)
{
    if (!playing) return;
    srcRect.x = current_frame * frame_w;
    srcRect.y = row * frame_h;
    SDL_Rect destRect = {x, y, frame_w, frame_h};
    SDL_RenderCopy(Game::renderer, spritesheet, &srcRect, &destRect);
}

bool Animation::isPlaying()
{
    return playing;
}
