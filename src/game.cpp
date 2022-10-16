
#include <thread>
#include <chrono>
#include <iostream>
#include <random>

#include "game.h"
#include "spaceship.h"

using namespace asteroids;

bool Game::init()
{
    if (!renderer_.init())
        return false;
        
    DrawableRect rect;
    
    // Create a spaceship shape by a group of rectangles
    spaceship_ = std::make_shared<Spaceship>();
    rect.rect = {30, 0, 30, 30};
    rect.color = {255, 0, 0, 0};
    spaceship_->addRect(rect);
    rect.rect = {0, 30, 90, 30};
    rect.color = {255, 255, 0, 0};
    spaceship_->addRect(rect);

    spaceship_->setWidth(90);
    spaceship_->setHeight(60);

    spaceship_->setPose(Pose(WINDOW_WIDTH / 2, WINDOW_HEIGHT - spaceship_->getHeight() / 2));

    entities_.emplace_back(spaceship_);
}

void Game::render()
{
    renderer_.render(entities_);
}

void Game::run()
{
    int frame = 0;

    start_ = std::chrono::system_clock::now();
    
    //Event handler
    SDL_Event e;

    // The main game loop
    while (state_ == GameState::Running){

        // get the current time
        auto start = std::chrono::system_clock::now(); 
        processInput();
        update();
        render();
        auto sleep_time = MS_PER_FRAME_ - std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start).count();
        if (sleep_time > 0)
            std::this_thread::sleep_for(std::chrono::milliseconds(sleep_time));
        
        auto frame_duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start);

        auto actual_fps = 1000.0 / frame_duration.count();
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

    auto game_duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start_);

    // create a new asteroid every 5 seconds;
    if (game_duration.count()  % 10000 <= MS_PER_FRAME_)
        createAstroid();

    // Update all Steroids, delete ones that are out of scope (Blasted)
    auto it_s = asteroids_.begin();
    while ( it_s != asteroids_.end()){
        (*it_s)->updatePose();
        // check if missile is out of screen
        if ((*it_s)->isAlive()){
            entities_.emplace_back(*it_s);
            it_s++;
        }
        else{// remove it from the missiles_ vector (also deallocate it as it is no longer owned by anyone)
            asteroids_.erase(it_s);
        }
    }

    if (checkCollisions()){
        std::cout << "Spaceship is hit by an asteroid\nGAME - OVER!!!!" << std::endl;
        state_ = GameState::Stopped;
    }

    checkHits();
    
        
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

void Game::createAstroid()
{
    auto asteroid = std::make_shared<Asteroid>();
    DrawableRect rect;
    rect.color = {0, 0, 255, 0};
    rect.rect = {20,0,10,5};
    asteroid->addRect(rect);
    rect.rect = {10,5,30,10};
    asteroid->addRect(rect);
    rect.rect = {0,15,50,20};
    asteroid->addRect(rect);
    rect.rect = {10,35,30,10};
    asteroid->addRect(rect);
    rect.rect = {20,45,10,5};

    asteroid->addRect(rect);
    asteroid->setHeight(50);
    asteroid->setWidth(50);

    std::mt19937 mt(std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now()- start_).count());
    std::uniform_int_distribution<int> dist(0, std::max(WINDOW_HEIGHT, WINDOW_WIDTH));
    Pose pose;
    
    pose.x = dist(mt)% WINDOW_WIDTH; 
    pose.y = dist(mt) % WINDOW_HEIGHT;

    asteroid->setPose(Pose(dist(mt)% WINDOW_WIDTH, asteroid->getHeight() / 2));

    asteroid->setSpeed(Speed(dist(mt) % max_speed_, dist(mt)% max_speed_));
    
    asteroids_.emplace_back(asteroid);

}

// check Spaceship collision with asteroids
bool Game::checkCollisions(){
    
    // Check if there are any asteroids

    if (asteroids_.size() == 0){
        return false;
    }
    
    // Get the spaceship rectangles
    auto sp_rects = spaceship_->getRects();
    
    // Iterate through asteroids
    for (auto &asteroid:asteroids_){
        // get the Asteroid rects
        auto as_rects = asteroid->getRects();
        // iterate through Asterois rects
        for (auto &as_rect:as_rects){
            
            SDL_Rect rect1;
            rect1 = as_rect.rect;
            rect1.x += asteroid->getPose().x;
            rect1.y += asteroid->getPose().y;

            // Iterate through the spaceship rects & chack collisions with each asteroid rects
            for (auto & sp_rect:sp_rects){
                
                SDL_Rect rect2;
                rect2 = sp_rect.rect;
                rect2.x += spaceship_->getPose().x;
                rect2.y += spaceship_->getPose().y;
                
                // Check for collision
                if (SDL_HasIntersection(&(rect1),&(rect2)))
                    return true;

            }
        }
    }

    return false;
}

void Game::checkHits()
{
    
    // Check if there are any asteroids or missiles
    if (missiles_.size() == 0 || asteroids_.size() == 0){
        return ;
    }
    
    // Iterate through asteroids
    for (auto &asteroid:asteroids_){
        // get the Asteroid rects
            auto as_rects = asteroid->getRects();
        // iterate through Asteroid rects
        for (auto &as_rect:as_rects){
            
            SDL_Rect rect1;
            rect1 = as_rect.rect;
            rect1.x += asteroid->getPose().x;
            rect1.y += asteroid->getPose().y;

            
            for (auto &missile:missiles_){

                auto mi_rects = missile->getRects();

                // Iterate through missile rects & check collisions with each asteroid rects
                for (auto & mi_rect:mi_rects){
                
                    SDL_Rect rect2;
                    rect2 = mi_rect.rect;
                    rect2.x += missile->getPose().x;
                    rect2.y += missile->getPose().y;
                    
                    // Check for collision
                    if (SDL_HasIntersection(&(rect1),&(rect2))){
                        missile->explode();
                        asteroid->explode();
                        std::cout << "Missile Hit" << std::endl;                  
                    }
                }
            }
        }
    }
    return;
}