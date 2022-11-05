#include "missile.h"
#include "game.h"

using namespace asteroids;

Missile::Missile(Pose& pose) : alive_(true)
{ 
    width_ = 10;
    height_ = 20;

    pose_.x = pose.x;
    pose_.y = pose.y;
    
    DrawableRect rect;

    rect.rect = {0,  -1 * height_, width_, height_};
    rect.color = {128, 128, 0, 0};
    rects_.emplace_back(rect);

    collisions_.emplace_back(rect.rect);

    max_speed_ = 10;

    speed_.y = -5;
    speed_.x = 0; 
}
void Missile::update()
{
    pose_.x += speed_.x;
    pose_.y += speed_.y;
    
    if (pose_.y < 0)
        alive_ = false;
}