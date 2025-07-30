#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <vector>
#include "Global.h"

class Player
{
private:
    SDL_Texture *player_idle;
    SDL_Texture *player_walk;
    SDL_Texture *player;
    SDL_FRect player_rect;
    SDL_FRect texture_rect;
    SDL_FlipMode flip = SDL_FLIP_NONE; 

    std::vector<SDL_FRect> idleFrames;
    std::vector<SDL_FRect> walkFrames;
    std::vector<SDL_FRect>* frames = nullptr;

    bool isJumping = false;
    bool isGrounded = false;

    float jumpStrength = 300.0f;
    float verticalVelocity = 0.0f;
    float gravity = 625.0f;
    float horizontalVelocity = 0.03f;
public:
    Player();
    void drawPlayer(SDL_Renderer *r);
    void loadPlayer(SDL_Renderer *r);
    bool loadAnimation(std::vector<SDL_FRect> &frames, SDL_Texture *texture, int frame_width, SDL_FRect frameInfo);
    void end();
    void handleEvent(const SDL_Event &e);
    void update(double delta);
    void windowCollision();
};
