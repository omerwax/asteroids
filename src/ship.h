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
        void accelRight(){ speed_.x = std::min(MAX_SPEED, speed_.x + ACCEL); }
        void accelLeft(){ speed_.x = std::max(-1 * MAX_SPEED, speed_.x - ACCEL); }
        void accelDown(){ speed_.y = std::min(MAX_SPEED, speed_.y + ACCEL); }
        void accelUp(){ speed_.y = std::max(-1 * MAX_SPEED, speed_.y - ACCEL); }
        int getXSpeed(){ return speed_.x; }
        int getYSpeed(){ return speed_.y; }
        void updatePose(int &window_width, int &window_height);
        void stop(){ speed_ = Speed(0,0); }

    private:
        const int MAX_SPEED = 5;
        const int ACCEL = 1;
        Speed speed_;
        // std::vector<SDL_Rect> rects_;
    };

}

#endif