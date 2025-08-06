#pragma once
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <vector>
#include "Global.h"
#include "Texture.h"
#include "Animation.h"
#include <cassert>
#include "math.h"

class Player
{
private:
    Texture player_idle;
    Texture player_walk;
    SDL_Texture *player;
    SDL_FRect player_rect;
    SDL_FRect texture_rect;
    SDL_FlipMode flip = SDL_FLIP_NONE; 

    Animation idleAnimation;
    Animation walkAnimation;


    std::vector<SDL_FRect> idleFrames;
    std::vector<SDL_FRect> walkFrames;
    std::vector<SDL_FRect>* frames = nullptr;

    bool isJumping = false;
    bool isGrounded = false;

    float jumpStrength = 300.0f;
    float verticalVelocity = 0.0f;
    float gravity = 625.0f;
    float horizontalVelocity = 0.00f;
public:
    ~Player();
    Player();
    void draw(SDL_Renderer *r);
    void load(SDL_Renderer *r);
    void handleEvent(const SDL_Event &e);
    void update(double delta, std::vector<SDL_FRect> solidRects);
    bool windowCollision();
    bool checkCollision(SDL_FRect tile, SDL_FRect playerCollisionRect);
    void handleCollision(std::vector<SDL_FRect> solidRects, bool& groundedThisFrame);
    SDL_FRect getCollisionRect() const;
};
