#ifndef ASTEROIDS_MISSILE_H
#define ASTEROIDS_MISSILE_H

#include <string>
#include <vector>
#include <SDL2/SDL.h>

#include "moving_entity.h"

namespace asteroids
{
    
    class Missile : public MovingEntity {
    public:
        Missile() : alive_(true){ max_speed_ = 10; }
        void updatePose();
        void explode() { alive_ = false; }
        bool isAlive() { return alive_; }
    private:
        bool alive_;
        
    };

}

#endif