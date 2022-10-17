#include "asteroid.h"
#include "game.h"

using namespace asteroids;

void Asteroid::updatePose()
{
    pose_.x += speed_.x;
    pose_.x = std::max(0, pose_.x);
    pose_.x = std::min(pose_.x, WINDOW_WIDTH - width_ /2);

    // If We hit a wall reverse the speed
    if (pose_.x == (WINDOW_WIDTH - width_ / 2) || pose_.x == 0)
        speed_.x = -1 * speed_.x;
    
    pose_.y += speed_.y;
    pose_.y = std ::max(0, pose_.y);
    pose_.y = std::min(pose_.y, WINDOW_HEIGHT - height_ / 2);

    if (pose_.y == (WINDOW_HEIGHT - height_ / 2) || pose_.y ==  0)
        speed_.y = -1 * speed_.y;
}