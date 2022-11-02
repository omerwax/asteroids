#include "asteroid.h"
#include "game.h"

using namespace asteroids;


Asteroid::Asteroid(const int& pose_x, const int& pose_y) : alive_(true)
{ 
    max_speed_ = 10; accel_ = 1;
    width_ = 50, height_ = 50;
    pose_.x = pose_x; pose_.y = pose_y;

    DrawableRect rect;
    rect.color = {128, 0, 128, 0};
    
    rect.rect = {20,0,10,5};
    rects_.emplace_back(rect);
    rect.rect = {10,5,30,10};
    rects_.emplace_back(rect);
    rect.rect = {0,15,50,20};
    rects_.emplace_back(rect);
    rect.rect = {10,35,30,10};
    rects_.emplace_back(rect);
    rect.rect = {20,45,10,5};
    rects_.emplace_back(rect);

    
}

void Asteroid::updatePose()
{
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