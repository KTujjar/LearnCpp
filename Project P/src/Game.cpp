#include "Game.h"

Game::Game()
{
   renderWindow();
}

void Game::loadTextures()
{
    player.loadPlayer(renderer);
}

void Game::run()
{
    loadTextures();
    while(window != NULL)
    {
        processEvents();
        update();
        render();
    }
    end();
}

void Game::update()
{
    player.update();
}

void Game::render()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Clear to black
    SDL_RenderClear(renderer);




    player.drawPlayer(renderer);




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
        player.handleEvent(e);
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