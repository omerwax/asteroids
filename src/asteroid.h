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
        Asteroid() : alive_(true){}
        void updatePose();
        bool isAlive() { return alive_; }
    private:
        bool alive_;
        
    };

}

#endif