#ifndef ASTEROIDS_ASTEROID_H
#define ASTEROIDS_ASTEROID_H

#include <string>
#include <vector>
#include <SDL2/SDL.h>

#include "animated_entity.h"

#define SPRITE_SHEET_MATRIX_SIZE 8

namespace asteroids
{
    
    class Asteroid : public AnimatedEntity {
    public:
        Asteroid(SDL_Renderer * renderer);
        Asteroid() = delete;
        void update() override;
        void explode() { alive_ = false; }
        bool isAlive() { return alive_; }
    private:
        bool alive_;
        
    };

}

#endif