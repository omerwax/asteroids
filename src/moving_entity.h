#ifndef SPACE_INVADERS_MOVING_ENTITY_H
#define SPACE_INVADERS_MOVING_ENTITY_H

#include <string>
#include <vector>
#include <SDL2/SDL.h>

#include "drawable_entity.h"

namespace space_invaders
{
    
    
    class MovingEntity : public DrawableEntity {
    public:
        MovingEntity() : speed_(0, 0){}
        void setSpeed(Speed &s) { speed_ = s; }
        int getXSpeed(){ return speed_.x; }
        int getYSpeed(){ return speed_.y; }

    protected:
        const int MAX_SPEED = 5;
        const int ACCEL = 1;
        Speed speed_;
    };

}

#endif