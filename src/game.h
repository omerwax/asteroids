#ifndef ASTEROIDS_GAME_H
#define ASTEROIDS_GAME_H

#include <memory>
#include <chrono>

#include "renderer.h"
#include "drawable_entity.h"
#include "spaceship.h"
#include "asteroid.h"


#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720


namespace asteroids{

    enum class GameState{Running, Stopped};
    
     
    class Game{
    public:
        Game(int fps) : fps_(fps), width_(WINDOW_WIDTH), height_(WINDOW_HEIGHT), state_(GameState::Running){};
        bool init();
        void run();
        
    private:
        
        void processInput();
        void render();
        void update();
        void createAstroid();
        bool checkCollisions();
        void checkHits();
        
                
        std::shared_ptr<Spaceship> spaceship_;
        std::vector<std::shared_ptr<Missile>> missiles_;
        std::vector<std::shared_ptr<Asteroid>> asteroids_;
        std::vector<std::shared_ptr<DrawableEntity>> entities_;

        int fps_;
        const int width_ ;
        const int height_;
        const int MS_PER_FRAME_ = 1000 / fps_;
        const int max_speed_ = 10;
        Renderer renderer_{width_, height_};
        GameState state_;

        std::chrono::time_point<std::chrono::system_clock> start_;
        

    };
}

#endif