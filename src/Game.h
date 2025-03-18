#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include <SDL.h>

class Game
{
public:
    Game();
    ~Game();

    void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
    void handleEvents();
    void update();
    void render();
    void clean();
    bool running();

    static SDL_Renderer* renderer;

private:
    bool isRunning;
    SDL_Window* window;

};

#endif // GAME_H_INCLUDED
