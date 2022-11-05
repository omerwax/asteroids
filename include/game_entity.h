#ifndef ASTEROIDS_GAME_ENTITY_H
#define ASTEROIDS_GAME_ENTITY_H

#include <string>
#include <vector>
#include <utility>
#include <memory>
#include <SDL2/SDL.h>

namespace asteroids
{
    struct Pose{
        Pose(): x(0), y(0){}
        Pose(int x_, int y_) : x(x_), y(y_){}
        int x,y;
    };
    
    struct Speed{
        Speed() : x(0), y(0){}
        Speed(int x, int y) : x(x), y(y){}
        int x;
        int y;
        
    };

    class GameEntity{
    public:
               
        Pose getPose(){ return pose_;}
        void setPose(Pose &&pose){pose_ = pose;}
        int getWidth() { return width_; }
        int getHeight() { return width_; }
        void setSpeed(Speed &&s) { speed_ = s; }
        Speed getSpeed(){ return speed_; }
        bool intersects(std::shared_ptr< GameEntity> entity);
        void addCollision(SDL_Rect &&rect){ collisions_.emplace_back(rect); }
        virtual void update() = 0;


    protected:
        int width_, height_;
        std::vector<SDL_Rect> collisions_;
        Pose pose_;
        int max_speed_;
        int accel_;
        Speed speed_;
    };

}

#endif