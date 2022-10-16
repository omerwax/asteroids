#ifndef ASTEROIDS_MOVING_ENTITY_H
#define ASTEROIDS_MOVING_ENTITY_H

#include <string>
#include <vector>
#include <SDL2/SDL.h>

#include "drawable_entity.h"

namespace asteroids
{
    
    
    class MovingEntity : public DrawableEntity {
    public:
        MovingEntity() : speed_(0, 0){}
        void setSpeed(Speed &&s) { speed_.x = s.x % max_speed_; speed_.y = s.y % max_speed_ ;}
        int getXSpeed(){ return speed_.x; }
        int getYSpeed(){ return speed_.y; }
        void updatePose();

    protected:
        int max_speed_;
        int accel_;
        Speed speed_;
    };

}

#endif