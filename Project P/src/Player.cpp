#include "Player.h"


Player::Player()
{
}

void Player::handleEvent(const SDL_Event &e)
{
    if (e.type == SDL_EVENT_KEY_DOWN) {
        if (e.key.scancode == SDLK_SPACE && !isJumping) {
            isJumping = true;
        }
    } else if (e.type == SDL_EVENT_KEY_UP) {
        if (e.key.scancode == SDLK_SPACE) {
            isJumping = false;
        }
    }
}

void Player::update()
{
    int numkeys = 0;
    SDL_PumpEvents();
    const bool* currentKeyStates = SDL_GetKeyboardState(&numkeys);

    if (currentKeyStates[SDL_SCANCODE_D]) {
        player = player_walk;
        frames = &walkFrames;
        player_rect.x += 0.05; // Move right
        flip = SDL_FLIP_NONE;
    }
    else if (currentKeyStates[SDL_SCANCODE_A]) {
        player = player_walk;
        frames = &walkFrames;
        player_rect.x -= 0.05; // Move left
        flip = SDL_FLIP_HORIZONTAL;
    }
    else
    {
        player = player_idle;
        frames = &idleFrames;
    }
}

void Player::loadPlayer(SDL_Renderer *r)
{

    //////////////////////////// LOAD IDLE //////////////////////////////////
    player_idle = IMG_LoadTexture(r, "../assets/Soldier/Soldier-Idle.png");
    //Removes blur
    SDL_SetTextureScaleMode(player_idle, SDL_SCALEMODE_NEAREST);
    if (!player_idle) {
        SDL_Log("Failed to load player texture: %s", SDL_GetError());
    }

    //////////////////////////// LOAD WALK //////////////////////////////////
    player_walk = IMG_LoadTexture(r, "../assets/Soldier/Soldier-Walk.png");
    //Removes blur
    SDL_SetTextureScaleMode(player_walk, SDL_SCALEMODE_NEAREST);
    if (!player_walk) {
        SDL_Log("Failed to load player texture: %s", SDL_GetError());
    }

    //sets base animation to idle
    player = player_idle;
    //Rect that crops the texture 
    SDL_FRect idleInfo = {41, 38, 25, 25};
    SDL_FRect walkInfo = {41, 38, 25, 25};

    loadAnimation(idleFrames, player_idle, 100, idleInfo);
    loadAnimation(walkFrames, player_walk, 100, walkInfo);

    frames = &idleFrames;

    //Rect for the position to draw
    player_rect.x = 0;
    player_rect.y = 100;
    player_rect.w = 100;
    player_rect.h = 100;
}

//Takes in a vector of SDL_FRect, the texture, the base width of a single frame, and the frameRect to create a vector of all the frames to create an animation.
void Player::loadAnimation(std::vector<SDL_FRect> &frames, SDL_Texture *texture, int frame_width, SDL_FRect frameInfo)
{   
    SDL_FRect t;
    int frameCount = texture->w/frame_width;
    for(int i = 0; i < frameCount; i++)
    {
        t = {frameInfo.x + (frame_width * i), (float)frameInfo.y, (float)frameInfo.w, (float)frameInfo.h};
        //SDL_Log("%f", t.x);
        frames.push_back(t);
    }
}

void Player::drawPlayer(SDL_Renderer *r)
{
    Uint32 ticks = SDL_GetTicks();
    const Uint32 frameduration = 150;
    int currentFrame = (ticks/frameduration) % frames->size();
    SDL_FRect src = (*frames)[currentFrame];
    if (!SDL_RenderTextureRotated(r, player, &src, &player_rect, 0.0, nullptr, flip)) {
        SDL_Log("RenderTextureRotated failed: %s", SDL_GetError());
    }
}

void Player::end()
{
    SDL_DestroyTexture(player);
}