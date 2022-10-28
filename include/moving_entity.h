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
        void setSpeed(Speed &&s);
        int getXSpeed(){ return speed_.x; }
        int getYSpeed(){ return speed_.y; }
        virtual void updatePose() = 0;

    protected:
        int max_speed_;
        int accel_;
        Speed speed_;
    };

}

#endif