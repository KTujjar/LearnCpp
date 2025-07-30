#include "Player.h"
#include <cassert>


Player::Player()
{
}

void Player::handleEvent(const SDL_Event &e)
{
    if (e.type == SDL_EVENT_KEY_DOWN) {
        if (e.key.scancode == SDL_SCANCODE_SPACE && !isJumping && isGrounded) {
            isJumping = true;
            isGrounded = false;
            verticalVelocity = -jumpStrength;
        }
    } 
}

void Player::update(double delta)
{
    int numkeys = 0;
    SDL_PumpEvents();
    const bool* currentKeyStates = SDL_GetKeyboardState(&numkeys);

    //Player Controller
    if (currentKeyStates[SDL_SCANCODE_D]) {
        horizontalVelocity = 0.03f;
        player = player_walk.texture;
        frames = &walkFrames;
        player_rect.x += horizontalVelocity; // Move right
        flip = SDL_FLIP_NONE;
    }
    else if (currentKeyStates[SDL_SCANCODE_A]) {
        horizontalVelocity = 0.03f;
        player = player_walk.texture;
        frames = &walkFrames;
        player_rect.x -= horizontalVelocity; // Move left
        flip = SDL_FLIP_HORIZONTAL;
    }
    else
    {
        horizontalVelocity = 0.00;
        player = player_idle.texture;
        frames = &idleFrames;
    }

    //Jump
    if(isJumping || !isGrounded)
    {
        verticalVelocity += gravity * delta;
    }

    //VerticalVelocity
    player_rect.y += verticalVelocity * delta;

    //Player Window Collision
    windowCollision();
}

void Player::windowCollision()
{
    float left, right, top, bottom;
    float rightCollision = player_rect.w - (44.0f * (player_rect.w/100));
    float leftCollision = player_rect.w - (56.0f * (player_rect.w/100));
    float topCollision = player_rect.h - (57.0f * (player_rect.h/100));
    float botCollision = player_rect.h - (43.0f * (player_rect.h/100));

    left = player_rect.x + leftCollision;
    right = player_rect.x + rightCollision;
    top = player_rect.y;
    bottom = player_rect.y + botCollision;

    if(left < 0)
    {
        horizontalVelocity = 0;
        player_rect.x = 0 - leftCollision;
    }

    else if(right > Global::windowWidth)
    {
        horizontalVelocity = 0;
        player_rect.x = Global::windowWidth - rightCollision;
    }

    else if(bottom > Global::windowHeight)
    {
        isGrounded = true;
        isJumping = false;
        verticalVelocity = 0;
    }

    else if(top < 0)
    {
        player_rect.y = Global::windowHeight;
    }
}

void Player::loadPlayer(SDL_Renderer *r)
{

    //////////////////////////// LOAD IDLE //////////////////////////////////
    if(!player_idle.loadTexture(r, "../assets/Soldier/Soldier-Idle.png"))
    {
        SDL_Log("Failed to load player idle texture: %s", SDL_GetError());
    }

    //Removes blur
    SDL_SetTextureScaleMode(player_idle.texture, SDL_SCALEMODE_NEAREST);

    //////////////////////////// LOAD WALK //////////////////////////////////
    if(!player_walk.loadTexture(r, "../assets/Soldier/Soldier-Walk.png"))
    {
        SDL_Log("Failed to load player walk texture: %s", SDL_GetError());
    }
    //Removes blur
    SDL_SetTextureScaleMode(player_walk.texture, SDL_SCALEMODE_NEAREST);

    //sets base animation to idle
    player = player_idle.texture;
    //Rect that crops the texture 
    SDL_FRect idleInfo = {0, 0, 100, 100};
    SDL_FRect walkInfo = {0, 0, 100, 100};

    if(!loadAnimation(idleFrames, player_idle.texture, 100, idleInfo))
    {
        SDL_Log("idleFrames not loaded properly: %s", SDL_GetError());
    }
    if(!loadAnimation(walkFrames, player_walk.texture, 100, walkInfo))
    {
        SDL_Log("idleFrames not loaded properly: %s", SDL_GetError());
    }

    frames = &idleFrames;

    //Rect for the position to draw
    player_rect.x = 100;
    player_rect.y = 100;
    player_rect.w = 400;
    player_rect.h = 400;
}

//Takes in a vector of SDL_FRect, the texture, the base width of a single frame, and the frameRect to create a vector of all the frames to create an animation.
bool Player::loadAnimation(std::vector<SDL_FRect> &frames, SDL_Texture *texture, int frame_width, SDL_FRect frameInfo)
{   
    frames.clear();
    SDL_FRect t;
    float texWidth = 0, texHeight = 0;
    if (!SDL_GetTextureSize(texture, &texWidth, &texHeight)) 
    {
        SDL_Log("Failed to get texture size: %s", SDL_GetError());
        return false;
    }
    
    int frameCount = static_cast<int>(texWidth / frame_width);

    if(frameCount <= 0)
    {
        SDL_Log("Invalid frameCount = %d", frameCount);
        return false;
    }
    for(int i = 0; i < frameCount; i++)
    {
        t = {frameInfo.x + (frame_width * i), (float)frameInfo.y, (float)frameInfo.w, (float)frameInfo.h};
        //SDL_Log("%f", t.x);
        frames.push_back(t);
    }

    return true;
}

void Player::drawPlayer(SDL_Renderer *r)
{
    //Uint32 ticks = SDL_GetTicks();
    const Uint32 frameDuration = 100;
    if (!frames) {
        SDL_Log("Error: frames pointer is null");
        return;
    }

    size_t frameCount = frames->size();
    if (frameCount == 0) {
        SDL_Log("Error: frames vector is empty (size==0)");
        return;
    }

    if (frameDuration == 0) {
        SDL_Log("Error: frameduration is zero");
        return;
    }

    Uint32 ticks = SDL_GetTicks();
    int currentFrame = (ticks / frameDuration) % static_cast<int>(frameCount);
    SDL_FRect src = (*frames)[currentFrame];

    if (!SDL_RenderTextureRotated(r, player, &src, &player_rect, 0.0, nullptr, flip)) {
        SDL_Log("RenderTextureRotated failed: %s", SDL_GetError());
    }
}

void Player::end()
{
    SDL_DestroyTexture(player);
}