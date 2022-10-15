#ifndef SPACE_INVADERS_SPACESHIP_H
#define SPACE_INVADERS_SPACESHIP_H

#include <string>
#include <vector>
#include <memory>
#include <SDL2/SDL.h>


#include "moving_entity.h"
#include "missile.h"

namespace space_invaders
{
    
    class Spaceship : public MovingEntity {
    public:
        void accelRight(){ speed_.x = std::min(MAX_SPEED, speed_.x + ACCEL); }
        void accelLeft(){ speed_.x = std::max(-1 * MAX_SPEED, speed_.x - ACCEL); }
        void accelDown(){ speed_.y = std::min(MAX_SPEED, speed_.y + ACCEL); }
        void accelUp(){ speed_.y = std::max(-1 * MAX_SPEED, speed_.y - ACCEL); }
        void stop(){ speed_ = Speed(0,0); }
        void updatePose();
        std::shared_ptr<Missile> shoot();
        
            
    private:
        
    };

}

#endif