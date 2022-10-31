#ifndef ASTEROIDS_GAME_H
#define ASTEROIDS_GAME_H

#include <memory>
#include <chrono>
#include <random>

#include "renderer.h"
#include "drawable_entity.h"
#include "spaceship.h"
#include "asteroid.h"
#include "game_event.h"


#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define INITIAL_INTERVAL 5  
#define MIN_INTERVAL 1



namespace asteroids{

    enum class GameState{Init, Idle, PlayerName, Running, GameOver, Paused, End};
    
    class Game{
    public:
        Game(int fps) : fps_(fps), 
                        width_(WINDOW_WIDTH), height_(WINDOW_HEIGHT), 
                        state_(GameState::Init), score_(0), 
                        asteroids_interval_(INITIAL_INTERVAL),
                        level_(0){};
        bool init();
        void run();
        
    private:
        
        void processInput();
        void processEvents();
        void render();
        void update();
        void createSpaceship();
        void createAstroid();
        bool checkCollisions();
        void checkHits();
        void reset();
        void pause();
        void resume();
        void updateGameOver();
        void updateGameIdle();
        void updateGameRunning();
        void updateGamePlayerName();
        void updateHighScore();
        void createTexts(const int &countdown);
        int getTextSize(const int &font_size);
        
                
        std::shared_ptr<Spaceship> spaceship_;
        std::vector<std::shared_ptr<Missile>> missiles_;
        std::vector<std::shared_ptr<Asteroid>> asteroids_;
        std::shared_ptr<DrawableEntity> texts_;

        std::vector<GameEvent> events_;

        std::string player_name_;
        int fps_;
        int actual_fps_;
        int asteroids_interval_;
        int level_;
        int score_;
        int final_score_;
        const int width_ ;
        const int height_;
        const int MS_PER_FRAME_ = 1000 / fps_;
        const int max_speed_ = 10;
        int high_score_;
        std::string high_score_player_;
        Renderer renderer_{width_, height_};
        GameState state_;
        GameState prev_state_;

        std::shared_ptr<std::mt19937> mt_;
        std::chrono::time_point<std::chrono::system_clock> asteroid_time_;
        std::chrono::milliseconds asteroid_delta_;
        std::chrono::time_point<std::chrono::system_clock> start_;
                
    };
}

#endif