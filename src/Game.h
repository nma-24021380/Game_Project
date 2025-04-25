#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include <SDL.h>

enum class GameState {ADVENTURE, BATTLE, GAME_OVER, WIN};

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
    static SDL_Event event;

private:
    bool is_running;
    bool needs_cleanup;
    SDL_Window* window;
    GameState game_state;

};

#endif // GAME_H_INCLUDED
