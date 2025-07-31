#include "Game.h"

Game::Game()
{
   renderWindow();
}

void Game::loadTextures()
{
    player.loadPlayer(renderer);
    map.load(renderer, "../assets/Tiles/level3.tmx");
}

void Game::run()
{
    Uint64 last = SDL_GetPerformanceCounter();
    loadTextures();
    while(window != NULL)
    {
        Uint64 now = SDL_GetPerformanceCounter();
        double dt = double(now - last) / SDL_GetPerformanceFrequency();
        last = now;
        processEvents();
        update(dt);
        render();
    }
    end();
}

void Game::update(double delta)
{
    player.update(delta);
}

void Game::render()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Clear to black
    SDL_RenderClear(renderer);
    SDL_SetRenderLogicalPresentation(renderer, Global::windowWidth, Global::windowHeight, SDL_LOGICAL_PRESENTATION_INTEGER_SCALE); //scales renderer with window



    player.drawPlayer(renderer);
    map.draw(renderer);



    SDL_RenderPresent(renderer);
}

void Game::end()
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    player.end();
    SDL_Quit();
}

void Game::processEvents()
{
    while(SDL_PollEvent(&e))
    {
        if(e.type == SDL_EVENT_QUIT)
        {
            window = NULL;
            break;
        }
        handleEvent(e);
        player.handleEvent(e);
    }
}

void Game::handleEvent(const SDL_Event &e)
{
    if (e.type == SDL_EVENT_KEY_DOWN) {
        if (e.key.scancode == SDL_SCANCODE_F11) 
        {
            flags = SDL_GetWindowFlags(window);
            if(flags & SDL_WINDOW_FULLSCREEN)
                SDL_SetWindowFullscreen(window, 0);
            else
                SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
        }
    }   
}

void Game::renderWindow()
{
    if(!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    }
    else
    {
        //Create window
        if(SDL_CreateWindowAndRenderer( "SDL Tutorial", Global::windowWidth, Global::windowHeight, SDL_EVENT_WINDOW_SHOWN, &window, &renderer))
        {
            SDL_Log( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
        }
        else
        {
            //Get window surface
            screenSurface = SDL_GetWindowSurface( window );

            //Get pixel format of surface
            details = SDL_GetPixelFormatDetails( screenSurface->format );

            //Fill the surface white
            SDL_FillSurfaceRect( screenSurface, NULL, SDL_MapRGB( details, NULL, 0xFF, 0xFF, 0xFF ));
            
            //Update the surface
            SDL_UpdateWindowSurface( window );

            //Hack to get window to stay up
        }
    }
}