#ifndef ASTEROIDS_MISSILE_H
#define ASTEROIDS_MISSILE_H

#include <string>
#include <vector>
#include <SDL2/SDL.h>

#include "rects_entity.h"


namespace asteroids
{
    
    class Missile : public RectsEntity {
    public:
        Missile(Pose& pose);
        Missile() = delete;
        void update() override;
        void explode() { alive_ = false; }
        bool isAlive() { return alive_; }
    private:
        bool alive_;
                
    };

}

#endif