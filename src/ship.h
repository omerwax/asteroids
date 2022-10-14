#ifndef SPACE_INVADERS_SHIP_H
#define SPACE_INVADERS_SHIP_H

#include <string>
#include <vector>
#include <SDL2/SDL.h>

#include "drawable_entity.h"

namespace space_invaders
{
    struct Speed{
        int x;
        int y;
        Speed(int x, int y) : x(x), y(y){}
    };
    
    class Ship : public DrawableEntity {
    public:
        Ship() : speed_(0, 0){}
        void setXSpeed(int speed){speed_.x = speed;}
        int getXSpeed(){ return speed_.x; }
        void setYSpeed(int speed){speed_.y = speed;}
        int getYSpeed(){ return speed_.y; }

    private:
        Speed speed_;
        // std::vector<SDL_Rect> rects_;
    };

}

#endif