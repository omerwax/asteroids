#ifndef ASTEROIDS_SPACESHIP_H
#define ASTEROIDS_SPACESHIP_H

#include <string>
#include <vector>
#include <memory>
#include <SDL2/SDL.h>


#include "moving_entity.h"
#include "missile.h"

namespace asteroids
{
    
    class Spaceship : public MovingEntity {
    public:
        Spaceship() {max_speed_ = 3; accel_ = 1;}
        void accelRight(){ speed_.x = std::min(max_speed_, speed_.x + accel_); }
        void accelLeft(){ speed_.x = std::max(-1 * max_speed_, speed_.x - accel_); }
        void accelDown(){ speed_.y = std::min(max_speed_, speed_.y + accel_); }
        void accelUp(){ speed_.y = std::max(-1 * max_speed_, speed_.y - accel_); }
        void stop(){ speed_ = Speed(0,0); }
        void updatePose();
        std::shared_ptr<Missile> shoot();
        
            
    private:
        
    };

}

#endif