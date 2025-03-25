#ifndef GRAPHICS_H_INCLUDED
#define GRAPHICS_H_INCLUDED

#include <SDL.h>

class Animation
{
public:
    Animation();
    Animation(const char* file, int w, int h, int total, int speed);

    void start();
    void update();
    void render(int x, int y, int row = 0);
    bool isPlaying();

private:
    SDL_Texture* spriteSheet;
    SDL_Rect srcRect;

    int frameW, frameH;
    int currentFrame = 0;
    int totalFrames;
    int frameSpeed;
    bool playing = false;

};

#endif // GRAPHICS_H_INCLUDED
