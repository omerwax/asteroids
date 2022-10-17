#ifndef ASTEROIDS_ASTEROID_H
#define ASTEROIDS_ASTEROID_H

#include <string>
#include <vector>
#include <SDL2/SDL.h>

#include "moving_entity.h"

namespace asteroids
{
    
    class Asteroid : public MovingEntity {
    public:
        Asteroid() : alive_(true){ max_speed_ = 5; accel_ = 1; }
        void updatePose() override;
        void explode() { alive_ = false; }
        bool isAlive() { return alive_; }
    private:
        bool alive_;
        
    };

}

#endif