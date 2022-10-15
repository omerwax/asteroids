#ifndef SPACE_INVADERS_GAME_H
#define SPACE_INVADERS_GAME_H

#include <memory>

#include "renderer.h"
#include "drawable_entity.h"
#include "spaceship.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720


namespace space_invaders{

    enum class GameState{Running, Stopped};
    
     
    class Game{
    public:
        Game(int fps) : fps_(fps), width_(WINDOW_WIDTH), height_(WINDOW_HEIGHT), state_(GameState::Running){};
        bool init();
        void run();
        void processInput();
        void render();
        void update();
    private:
        std::shared_ptr<Spaceship> spaceship_;
        std::vector<std::shared_ptr<Missile>> missiles_;
        int fps_;
        const int width_ ;
        const int height_;
        const int MS_PER_FRAME_ = 1000 / fps_;
        const int MAX_SPEED = 10;
        Renderer renderer_{width_, height_};
        GameState state_;
        std::vector<std::shared_ptr<MovingEntity>> entities_;

    };
}

#endif