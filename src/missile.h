#ifndef SPACE_INVADERS_MISSILE_H
#define SPACE_INVADERS_MISSILE_H

#include <string>
#include <vector>
#include <SDL2/SDL.h>

#include "moving_entity.h"

namespace space_invaders
{
    
    class Missile : public MovingEntity {
    public:
        Missile() : alive_(true){}
        void updatePose();
        bool isAlive() { return alive_; }
    private:
        bool alive_;
        
    };

}

#endif