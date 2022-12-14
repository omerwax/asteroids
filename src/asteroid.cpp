#include <SDL2/SDL.h>
#include "asteroid.h"

#include "game.h"

using namespace asteroids;


Asteroid::Asteroid(SDL_Renderer * renderer) : alive_(true)
{ 
    max_speed_ = 10; accel_ = 1;
    width_ = 50, height_ = 50;
    pose_ = Pose(0,0);

    SDL_Rect rect{24, 24, 80, 80};
    collisions_.emplace_back(rect);

    renderer_ = renderer;

    loadSpriteSheet("../img/asteroid_sprite.png", SPRITE_SHEET_MATRIX_SIZE);

    this->animation_fps_ = 15;
       
}

void Asteroid::update()
{
    
    updateAnimationIndex();

    // Calculate the new pose
    pose_.x += speed_.x;
    // Trim it at the edges
    pose_.x = std::max(0, pose_.x);
    pose_.x = std::min(pose_.x, WINDOW_WIDTH - width_ /2);

    // If We hit a wall reverse the speed
    if (pose_.x == (WINDOW_WIDTH - width_ / 2) || pose_.x == 0)
        speed_.x = -1 * speed_.x;
    
    // repeat for y
    pose_.y += speed_.y;
    pose_.y = std ::max(0, pose_.y);
    pose_.y = std::min(pose_.y, WINDOW_HEIGHT - height_ / 2);

    if (pose_.y == (WINDOW_HEIGHT - height_ / 2) || pose_.y ==  0)
        speed_.y = -1 * speed_.y;
}