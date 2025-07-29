#include <SDL3/SDL.h>
#include "Global.h"
#include "Player.h"

class Game
{
private:
    void update();
    void render();
    void end();
    void renderWindow();
    void processEvents();
    void loadTextures();

    //Initialize window variables
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Surface* screenSurface = NULL;
    SDL_Event e;   
    const SDL_PixelFormatDetails* details = NULL;
    Player player;
public:
    Game();
    void run();
};
