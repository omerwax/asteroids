#ifndef ASTEROIDS_ASTEROID_H
#define ASTEROIDS_ASTEROID_H

#include <string>
#include <vector>
#include <SDL2/SDL.h>

#include "rects_entity.h"

namespace asteroids
{
    
    class Asteroid : public RectsEntity {
    public:
        Asteroid(Pose& pose);
        Asteroid() = delete;
        void update() override;
        void explode() { alive_ = false; }
        bool isAlive() { return alive_; }
    private:
        bool alive_;
        
    };

}

#endif