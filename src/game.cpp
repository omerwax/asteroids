
#include <thread>
#include <chrono>
#include <iostream>
#include <random>

#include "game.h"
#include "spaceship.h"



using namespace space_invaders;

bool Game::init()
{
    if (!renderer_.init())
        return false;
    
    
    DrawableRect rect;
    // Define the spaceship shape by a group of rectangles
    spaceship_ = std::make_shared<Spaceship>();
    rect.rect = {30, 0, 30, 30};
    rect.color = {255, 0, 0, 0};
    spaceship_->addRect(rect);
    rect.rect = {0, 30, 90, 30};
    rect.color = {255, 255, 0, 0};
    spaceship_->addRect(rect);


    // Add the spaceship image
    int w, h;
    spaceship_->setImage("/home/omerwax/courses/Udacity/cpp-nanodegree/Capstone-project/images/xwing.bmp");
    
    // get image size from the renderer
    renderer_.getImageSize(spaceship_->getImagePath(), w, h);

    spaceship_->setWidth(w);
    spaceship_->setHeight(h);

    entities_.emplace_back(spaceship_);
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
       
    entities_.clear();

    // update spaceship pose
    spaceship_->updatePose();
    entities_.emplace_back(spaceship_);


    // Update all missile, delete ones that are out of scope(screen)
    auto it = missiles_.begin();
    while ( it != missiles_.end()){
        (*it)->updatePose();
        // check if missile is out of screen
        if ((*it)->isAlive()){
            entities_.emplace_back(*it);
            it++;
        }
        else{// remove it from the missiles_ vector (also deallocate it as it is no longer owned by anyone)
            missiles_.erase(it);
        }
    }
        
       
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
            spaceship_->accelRight();
            break;
        case SDLK_LEFT:
            spaceship_->accelLeft();
            break;
        case SDLK_UP:
            spaceship_->accelUp();
            break;
        case SDLK_DOWN:
            spaceship_->accelDown();
            break;
        case SDLK_SPACE:
            missiles_.emplace_back(spaceship_->shoot());
            break;
        default:
            break;
        }
    }
    
    return;

}