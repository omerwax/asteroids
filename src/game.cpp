
#include <thread>
#include <chrono>
#include <iostream>
#include <random>

#include "game.h"
#include "ship.h"


using namespace space_invaders;

bool Game::init()
{
    if (!renderer_.init())
        return false;
    
    ship_.setImage("/home/omerwax/courses/Udacity/cpp-nanodegree/Capstone-project/images/xwing.bmp");

    int w, h;

    renderer_.getImageSize(ship_.getImagePath(), w, h);

    ship_.setWidth(w);
    ship_.setHeight(h);
}

void Game::render()
{
    //renderer_.display_image("/home/omerwax/courses/Udacity/cpp-nanodegree/Capstone-project/images/image.bmp");
    renderer_.render(entities_);
}

void Game::run()
{
    int frame = 0;

    //Main loop flag

    //Event handler
    SDL_Event e;

    while (state_ == GameState::Running){

              
        // The main game loop

        // get the current time
        auto start = std::chrono::system_clock::now(); 
        processInput();
        update();
        render();
        auto sleep_time = MS_PER_FRAME_ - std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start).count();
        if (sleep_time > 0)
            std::this_thread::sleep_for(std::chrono::milliseconds(sleep_time));
        
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start);

        auto actual_fps = 1000.0 / duration.count();
        ++frame;
        if (actual_fps < fps_ - 5)
            std::cout << "Frame#" << frame << ", Actual fps = " << actual_fps << std::endl;
    }

}

void Game::update()
{
        
    auto pose = ship_.getPose();
    pose.x += ship_.getXSpeed();
    pose.x = std::min(pose.x, width_ - ship_.getWidth());
    pose.x = std::max(pose.x, 0);

    pose.y += ship_.getYSpeed();
    pose.y = std::min(pose.y, height_ - ship_.getHeight());
    pose.y = std::max(pose.y, 0);

    ship_.setPose(std::move(pose));

    
    entities_.clear();
    entities_.emplace_back(ship_); 
    
}


void Game::processInput(){
    //Handle events on queue
    SDL_Event e;
    SDL_PollEvent(&e);
    
    // First check for quit event
    if (e.type == SDL_QUIT){
        state_ = GameState::Stopped;
        return;
    }

    // a key is pressed
    if (e.type == SDL_KEYDOWN){
        switch (e.key.keysym.sym)
        {
        case SDLK_RIGHT:
            ship_.setXSpeed(SPEED);
            break;
        case SDLK_LEFT:
            ship_.setXSpeed(-1 * SPEED);
            break;
        case SDLK_UP:
            ship_.setYSpeed(-1 * SPEED);
            
            break;
        case SDLK_DOWN:
            ship_.setYSpeed(SPEED);
            break;
        default:
            ship_.setXSpeed(0);
            ship_.setYSpeed(0);
            break;
        }
    }
    
    if (e.type == SDL_KEYUP){
        // Todo replace with stop!
        ship_.setXSpeed(0);
        ship_.setYSpeed(0);

    }
        
    
    return;

}