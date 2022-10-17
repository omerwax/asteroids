#include "moving_entity.h"

using namespace asteroids;

void MovingEntity::setSpeed(Speed &&s) 
{ 
    speed_.x = std::min(s.x, max_speed_); 
    speed_.y = std::min(s.y, max_speed_);
}
