#include "Player.h"


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

void Player::update(double delta, std::vector<SDL_FRect> solidRects)
{
    int numkeys = 0;
    SDL_PumpEvents();
    const bool* currentKeyStates = SDL_GetKeyboardState(&numkeys);

    //Player Controller
    
    if (currentKeyStates[SDL_SCANCODE_D]) {
        horizontalVelocity = 100.0f;
        player = walkAnimation.texture;
        frames = &walkAnimation.frames;
        flip = SDL_FLIP_NONE;
    }
    else if (currentKeyStates[SDL_SCANCODE_A]) {
        horizontalVelocity = -100.0f;
        player = walkAnimation.texture;
        frames = &walkAnimation.frames;
        flip = SDL_FLIP_HORIZONTAL;
    }
    else
    {
        horizontalVelocity = 0.0f;
        player = idleAnimation.texture;
        frames = &idleAnimation.frames;
    }

    //Jump
    if(!isGrounded)
    {
        verticalVelocity += gravity * delta;
    }

    //VerticalVelocity and Horizontal velocity
    player_rect.y += verticalVelocity * delta;
    player_rect.x += horizontalVelocity * delta;
    
    //Player Collision
    bool groundedThisFrame = false;
    if(windowCollision())
    {
        player_rect.x = 100;
        player_rect.y = 100;
        horizontalVelocity = 0.0f;
    }
    handleCollision(solidRects, groundedThisFrame);
    isGrounded = groundedThisFrame;
    SDL_Log("IsGrounded After: %d", isGrounded);
}

bool Player::windowCollision()
{
    SDL_FRect playerCollisionRect = getCollisionRect();

    //left
    if (playerCollisionRect.x < 0) {
        return true;
    }
    //right
    else if (playerCollisionRect.x + playerCollisionRect.w > Global::windowWidth) {
        return true;
    }
    //bottom
    else if (playerCollisionRect.y + playerCollisionRect.h > Global::windowHeight) {
        return true;
    }
    //top
    else if (playerCollisionRect.y < 0) {
        return true;
    }
    return false;
}

SDL_FRect Player::getCollisionRect() const {
    float scaleX = player_rect.w / 100.0f;
    float scaleY = player_rect.h / 100.0f;

    float visibleX = 42.0f * scaleX;
    float visibleY = 38.0f * scaleY;
    float visibleW = 15.0f * scaleX;
    float visibleH = 20.0f * scaleY;

    return SDL_FRect {
        player_rect.x + visibleX,
        player_rect.y + visibleY,
        visibleW,
        visibleH
    };
}

bool Player::checkCollision(SDL_FRect tile, SDL_FRect playerCollisionRect)
{
    if (SDL_HasRectIntersectionFloat(&playerCollisionRect, &tile))
    {
        // SDL_Log("Player x: %f  Tile x: %f", playerCollisionRect.x, tile.x);
        // SDL_Log("Collision");
        return true;
    }
    return false;
}

void Player::handleCollision(std::vector<SDL_FRect> solidRects, bool& groundedThisFrame)
{
    SDL_FRect playerCollisionRect = getCollisionRect();

    float offsetX = playerCollisionRect.x - player_rect.x;
    float offsetY = playerCollisionRect.y - player_rect.y;
    
     
    

    for (const auto& tile : solidRects)
    {
        if (checkCollision(tile, playerCollisionRect))
        {
            SDL_FRect result;
            SDL_GetRectIntersectionFloat(&playerCollisionRect, &tile, &result);

            float feetY = playerCollisionRect.y + playerCollisionRect.h;
            float tileTopY = tile.y;
            
            float dx = 0;
            float dy = 0;
           
            if (result.w < result.h) {
                if (horizontalVelocity > 0) {
                    dx = -result.w;
                } else if (horizontalVelocity < 0) {
                    dx = result.w;  
                }
                horizontalVelocity = 0.0f;
            } else if(result.h < result.w) {
                if (verticalVelocity >= 0) {
                    dy = -result.h;
                    groundedThisFrame = true;
                    isJumping = false;
                    verticalVelocity = 0;
                } else if (verticalVelocity < 0) {
                    dy = result.h;
                    verticalVelocity = 0;
                    //isGrounded = false;
                    //groundedThisFrame = false;
                }
            }

            // Apply movement to the collision box
            playerCollisionRect.x += dx;
            playerCollisionRect.y += dy;

            // Recalculate player_rect based on collision rect and original offset
            player_rect.x = playerCollisionRect.x - offsetX;
            player_rect.y = playerCollisionRect.y - offsetY;

            // Recalculate playerCollisionRect to reflect new position
            playerCollisionRect = getCollisionRect();
        }
    }
}

void Player::load(SDL_Renderer *r)
{

    ///////////////////////////////// LOAD TEXTURES //////////////////////////////////////
    if(!idleAnimation.texture.loadTexture(r, "../assets/Soldier/Soldier-Idle.png"))
    {
        SDL_Log("Failed to load player idle texture: %s", SDL_GetError());
    }

    SDL_SetTextureScaleMode(idleAnimation.texture, SDL_SCALEMODE_NEAREST);

    if(!walkAnimation.texture.loadTexture(r, "../assets/Soldier/Soldier-Walk.png"))
    {
        SDL_Log("Failed to load player idle texture: %s", SDL_GetError());
    }

    SDL_SetTextureScaleMode(walkAnimation.texture, SDL_SCALEMODE_NEAREST);

    player = idleAnimation.texture;

    //////////////////////////////// LOAD ANIMATION FRAMES /////////////////////////////////
    if(idleAnimation.load(100.f, 100.f))
    {
        SDL_Log("idleFrames not loaded properly: %s", SDL_GetError());
    }

    if(walkAnimation.load(100.f, 100.f))
    {
        SDL_Log("walkFrames not loaded properly: %s", SDL_GetError());
    }

    frames = &idleAnimation.frames;

    //Rect for the position to draw
    player_rect.x = 100;
    player_rect.y = 100;
    player_rect.w = 200;
    player_rect.h = 200;
}

void Player::draw(SDL_Renderer *r)
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

    //sees player collision box
    SDL_SetRenderDrawColor(r, 0, 255, 0, 255); // Green
    SDL_FRect box = getCollisionRect();
    SDL_RenderRect(r, &box);
}

Player::~Player()
{
    SDL_DestroyTexture(player);
}