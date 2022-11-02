#include "missile.h"
#include "game.h"

using namespace asteroids;

Missile::Missile(int pose_x, int pose_y) : alive_(true)
{ 
    width_ = 10;
    height_ = 20;

    pose_.x = pose_x;
    pose_.y = pose_y;
    
    DrawableRect rect;

    rect.rect = {0,  -1 * height_, width_, height_};
    rect.color = {128, 128, 0, 0};
    rects_.emplace_back(rect);

    max_speed_ = 10;

    speed_.y = -5;
    speed_.x = 0; 
}
void Missile::updatePose()
{
    pose_.x += speed_.x;
    pose_.y += speed_.y;
    
    if (pose_.y < 0)
        alive_ = false;
}