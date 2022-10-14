#ifndef SPACE_INVADERS_GAME_H
#define SPACE_INVADERS_GAME_H

#include <memory>

#include "renderer.h"
#include "drawable_entity.h"
#include "ship.h"

namespace space_invaders{

    enum class GameState{Running, Stopped};
    
     
    class Game{
    public:
        Game(int width, int height, int fps) : fps_(fps), width_(width), height_(height), state_(GameState::Running){};
        bool init();
        void run();
        void processInput();
        void render();
        void update();
    private:
        Ship ship_;
        int fps_;
        const int width_ ;
        const int height_;
        const int MS_PER_FRAME_ = 1000 / fps_;
        const int MAX_SPEED = 10;
        Renderer renderer_{width_, height_};
        std::vector<DrawableEntity> entities_;
        GameState state_;


    };
}

#endif