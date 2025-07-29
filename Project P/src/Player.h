#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <vector>

class Player
{
private:
    SDL_Texture *player_idle;
    SDL_Texture *player_walk;
    SDL_Texture *player;
    SDL_FRect player_rect;
    SDL_FRect texture_rect;
    bool isJumping = false;
    SDL_FlipMode flip = SDL_FLIP_NONE; 
    std::vector<SDL_FRect> idleFrames;
    std::vector<SDL_FRect> walkFrames;
    std::vector<SDL_FRect>* frames;
public:
    Player();
    void drawPlayer(SDL_Renderer *r);
    void loadPlayer(SDL_Renderer *r);
    void loadAnimation(std::vector<SDL_FRect> &frames, SDL_Texture *texture, int frame_width, SDL_FRect frameInfo);
    void end();
    void handleEvent(const SDL_Event &e);
    void update();
};
