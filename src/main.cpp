#include "Game.h"
#include "Defs.h"
#include <ctime>
#include <cstdlib>

Game* game = nullptr;

int main(int argc, char* argv[])
{
    srand(time(0));

    game = new Game();
    game->init("Game Project", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, false);
    while (game->running())
    {
        game->handleEvents();
        game->update();
        game->render();
    }
    game->clean();

    return 0;
}
