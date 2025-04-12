#ifndef GRAPHICS_H_INCLUDED
#define GRAPHICS_H_INCLUDED

#include <SDL.h>

class Animation
{
public:
    Animation();
    Animation(const char* file, int w, int h, int total, int speed);

    void start();
    void update(bool loop = true);
    void render(int x, int y, int row = 0);
    bool isPlaying();

private:
    SDL_Texture* spritesheet;
    SDL_Rect srcRect;

    int frame_w, frame_h;
    int current_frame = 0;
    int total_frames;
    int frame_speed;
    bool playing = false;

};

#endif // GRAPHICS_H_INCLUDED
