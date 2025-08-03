#include <SDL3/SDL.h>
#include "Global.h"
#include "Player.h"
#include "Map.h"
#include "Viewport.h"

class Game
{
private:
    void update(double delta);
    void render();
    void end();
    void renderWindow();
    void processEvents();
    void loadTextures();
    void handleEvent(const SDL_Event &e);

    //Initialize window variables
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Surface* screenSurface = NULL;
    SDL_Event e;   
    const SDL_PixelFormatDetails* details = NULL;
    Player player;
    Map map;
    Viewport viewport;

    Uint32 flags;

    SDL_DisplayMode *windowed;
    SDL_DisplayMode *fullscreen;
public:
    Game();
    void run();
};
