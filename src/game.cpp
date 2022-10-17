
#include <thread>
#include <chrono>
#include <iostream>
#include <sstream>

#include "game.h"
#include "spaceship.h"

using namespace asteroids;

bool Game::init()
{
    
    // Initialize the renderer
    if (!renderer_.init())
        return false;
        
    // Initialize the random engine
    mt_ = std::make_shared<std::mt19937>(std::chrono::system_clock::now().time_since_epoch().count());
    
      
    // Define spaceship shape by a group of rectangles
    DrawableRect rect;
    spaceship_ = std::make_shared<Spaceship>();
    rect.rect = {30, 0, 30, 30};
    rect.color = {255, 0, 0, 0};
    spaceship_->addRect(std::move(rect));
    rect.rect = {0, 30, 90, 30};
    rect.color = {255, 255, 0, 0};
    spaceship_->addRect(std::move(rect));

    // set initial pose
    spaceship_->setWidth(90);
    spaceship_->setHeight(60);

    spaceship_->setPose(Pose(WINDOW_WIDTH / 2, WINDOW_HEIGHT - spaceship_->getHeight() / 2));

    //create the first asteroid;
    createAstroid();

    state_ = GameState::Idle;

    return true;
}

void Game::render()
{
    renderer_.render(entities_);
}

void Game::run()
{
    int frame = 0;

    start_ = std::chrono::system_clock::now();
    asteroid_time_ = start_; shot_time_ = start_;

    
    //Event handler
    SDL_Event e;

    // The main game loop
    while (state_ != GameState::End){

        // get the current time
        auto start = std::chrono::system_clock::now(); 
        processInput();
        update();
        render();
        auto sleep_time = MS_PER_FRAME_ - std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start).count();
        if (sleep_time > 0)
            std::this_thread::sleep_for(std::chrono::milliseconds(sleep_time));
        
        auto frame_duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start);

        if (frame % fps_ == 0)
            actual_fps_ = 1000.0 / frame_duration.count();
        ++frame;
        if (actual_fps_ < fps_ - 5)
            std::cout << "Frame#" << frame << ", Actual fps = " << actual_fps_ << std::endl;
    }

}

// Update function update the position of all the game entities, check collisions and missile hits
// In addition it populates the entities_ vector with entities for rendering. It also 
void Game::update()
{
       
    entities_.clear();

    //Create the enities for the game start screen
    if (state_ == GameState::Idle)
    {
        auto message = std::make_shared<DrawableEntity>();
        DrawableText message_text;
        std::stringstream  text;
        text  << "Asteroids!!! Press Any key to start";
        message_text.text = text.str();
        message_text.color = {255, 255, 255, 0};
        message_text.rect = {WINDOW_WIDTH / 2 - WINDOW_WIDTH / 4 , 0, WINDOW_WIDTH / 2, 60};

        message->addText(std::move(message_text));
        entities_.emplace_back(message);
        return;

    }

    // Create the enities for the gameover screen
    if (state_ == GameState::GameOver)
    {
        auto message = std::make_shared<DrawableEntity>();
        DrawableText message_text;
        std::stringstream  text;
        text  << "Press the ENTER key to start again";
        message_text.text = text.str();
        message_text.color = {255, 255, 255, 0};
        message_text.rect = {WINDOW_WIDTH / 4 , WINDOW_HEIGHT - 80, WINDOW_WIDTH / 2, 60};
        message->addText(std::move(message_text));

        text.str("");
        text << "GAME OVER!!!";
        message_text.text = text.str();
        message_text.rect = {WINDOW_WIDTH * 3 / 8 , WINDOW_HEIGHT / 2 - 80, WINDOW_WIDTH / 4, 60};
                
        message->addText(std::move(message_text));

        text.str("");
        text << "SCORE: " << final_score_;
        message_text.text = text.str();
        message_text.rect = {WINDOW_WIDTH * 3 / 8 , WINDOW_HEIGHT / 2 + 20, WINDOW_WIDTH / 4, 60};
                
        message->addText(std::move(message_text));
        entities_.emplace_back(message);
        // set spaceship to initial pose
        return;

    }
    else if (state_ == GameState::Running){
        // update spaceship pose
        spaceship_->updatePose();
        entities_.emplace_back(spaceship_);

        // update level according to score
        if (score_ % 1000 == 0 && score_ != 0)
            level_ ++;


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

        auto game_duration = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - asteroid_time_);

        // create a new asteroid every interval seconds / interval decreases as level rises;
        if (game_duration.count() > std::max(asteroids_interval_ - level_, MIN_INTERVAL )){
            createAstroid();
            asteroid_time_ = std::chrono::system_clock::now();
        }
            

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

        // Check if the spaceship collided with an asteroid
        if (checkCollisions()){
            std::cout << "Spaceship is hit by an asteroid\nGAME - OVER!!!!" << std::endl;
            final_score_ = score_;
            state_ = GameState::GameOver;
        }

        // Check for missile hits
        checkHits();

        
        // Create the score entity
        auto message = std::make_shared<DrawableEntity>();
        DrawableText message_text;
        std::stringstream  text;
        text  << "SCORE: " << score_ ;
        message_text.text = text.str();
        message_text.color = {255, 255, 255, 0};
        message_text.rect = {0, 0, 320, 60};

        message->addText(std::move(message_text));
        
        
        // Create the FPS entity
        text.str("");
        text.clear();

        text << "FPS: " << actual_fps_;
        message_text.text = text.str();
        message_text.rect = {960, 0, 320, 60};

        message->addText(std::move(message_text));
        
        
        entities_.emplace_back(message);

    }
             
}


// Processing user input according to game state
void Game::processInput(){
    //Handle events on queue
    SDL_Event e;
    SDL_PollEvent(&e);
    
    // First check for quit event
    if (e.type == SDL_QUIT){
        state_ = GameState::End;
        return;
    }

    if (state_ == GameState::Running){
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
                // Limit shooting speed
                if (std::chrono::duration_cast<std::chrono::milliseconds>
                    (std::chrono::system_clock::now() - shot_time_).count() > MIN_SHOOTING_SPEED_MS)
                {
                    missiles_.emplace_back(spaceship_->shoot());
                    shot_time_ = std::chrono::system_clock::now();
                }
                break;
            default:
                break;
            }
        }
    }
    // Any key to start playing
    else if (state_ == GameState::Idle){
        // a key is pressed
        if (e.type == SDL_KEYDOWN){
            state_ = GameState::Running;
        }
    }

    // Return to start again
    else if (state_ == GameState::GameOver){
        // a key is pressed
        this->reset();
        if (e.type == SDL_KEYDOWN){
            if (e.key.keysym.sym == SDLK_RETURN)
            state_ = GameState::Running;
        }
    }


    return;

}

// Cretae a new astroid on a random place on the top row of the screen, with a random speed
void Game::createAstroid()
{
    auto asteroid = std::make_shared<Asteroid>();
    DrawableRect rect;
    rect.color = {0, 0, 255, 0};
    
    rect.rect = {20,0,10,5};
    asteroid->addRect(std::move(rect));
    rect.rect = {10,5,30,10};
    asteroid->addRect(std::move(rect));
    rect.rect = {0,15,50,20};
    asteroid->addRect(std::move(rect));
    rect.rect = {10,35,30,10};
    asteroid->addRect(std::move(rect));
    rect.rect = {20,45,10,5};
    asteroid->addRect(std::move(rect));

    asteroid->setHeight(50);
    asteroid->setWidth(50);

    std::uniform_int_distribution<int> dist(asteroid->getWidth(), WINDOW_WIDTH - asteroid->getWidth());
           
    asteroid->setPose(Pose(dist(*mt_),0));

    asteroid->setSpeed(Speed(dist(*mt_) % max_speed_, 2 + level_));
    
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

// Check for missile and asteroid hits
void Game::checkHits()
{
    
    // Check if there are any asteroids or missiles
    if (missiles_.size() == 0 || asteroids_.size() == 0){
        return ;
    }
     
    // Iterate through asteroids
    for (auto &missile:missiles_){
        
        // get the Asteroid rects
        auto mi_rects = missile->getRects();
        // iterate through missile rects
        for (auto &mi_rect:mi_rects){
            
            SDL_Rect rect1;
            rect1 = mi_rect.rect;
            rect1.x += missile->getPose().x;
            rect1.y += missile->getPose().y;

            for (auto &asteroid:asteroids_){

                // Make sure Asteroid was not already hit;
                if (asteroid->isAlive() == false)
                    continue;
                
                auto as_rects = asteroid->getRects();

                // Iterate through asteroid rects & check collisions with each asteroid rects
                for (auto & as_rect:as_rects){
                
                    SDL_Rect rect2;
                    rect2 = as_rect.rect;
                    rect2.x += asteroid->getPose().x;
                    rect2.y += asteroid->getPose().y;
                    
                    // Check for collision
                    if (SDL_HasIntersection(&(rect1),&(rect2))){
                        asteroid->explode();
                        missile->explode();
                        std::cout << "Missile Hit" << std::endl;  
                        score_ += 100;
                        // stop testing further rects as asteroid has already exploded
                        break;                
                    }
                }
            }
        }
    }
    
       
    return;
}

// reset the game variables
void Game::reset()
{
    spaceship_->setPose(Pose(WINDOW_WIDTH / 2, WINDOW_HEIGHT - spaceship_->getHeight() / 2));
    asteroids_.clear();
    createAstroid();
    asteroid_time_ = std::chrono::system_clock::now();
    score_ = 0;
    level_ = 0;
    asteroids_interval_ = INITIAL_INTERVAL;
}