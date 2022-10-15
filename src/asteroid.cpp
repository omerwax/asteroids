#include "asteroid.h"
#include "game.h"

using namespace asteroids;

void Asteroid::updatePose()
{
    pose_.x += speed_.x;
    // Check if hit a wall
    if (pose_.x > WINDOW_WIDTH - width_ / 2 || pose_.x < width_ / 2)
        speed_.x = -1 * speed_.x;
    
    pose_.y += speed_.y;

    if (pose_.y > WINDOW_HEIGHT - height_ / 2 || pose_.y < height_ / 2)
        speed_.y = -1 * speed_.y;

}