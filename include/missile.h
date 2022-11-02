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
        Missile(int pose_x, int pose_y);
        Missile() = delete;
        void updatePose() override;
        void explode() { alive_ = false; }
        bool isAlive() { return alive_; }
    private:
        bool alive_;
                
    };

}

#endif