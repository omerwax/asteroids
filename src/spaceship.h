#ifndef ASTEROIDS_SPACESHIP_H
#define ASTEROIDS_SPACESHIP_H

#include <string>
#include <vector>
#include <memory>
#include <chrono>
#include <SDL2/SDL.h>


#include "moving_entity.h"
#include "missile.h"

namespace asteroids
{
    
    class Spaceship : public MovingEntity {
    public:
        Spaceship(); 
        void accelRight(){ speed_.x = std::min(speed_.x + accel_, max_speed_); }
        void accelLeft(){ speed_.x = std::max(speed_.x - accel_,  - 1 * max_speed_); }
        void accelDown(){ speed_.y = std::min(speed_.y+ accel_, max_speed_); }
        void accelUp(){ speed_.y = std::max(speed_.y - accel_,  - 1 * max_speed_); }
        void stop(){ speed_ = Speed(0,0); }
        void updatePose();
        std::shared_ptr<Missile> shoot();
        
            
    private:

      
    };

}

#endif