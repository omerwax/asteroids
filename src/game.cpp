
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
    
    // Create the spaceship
    createSpaceship() ;
    
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
    asteroid_time_ = start_; 

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
       
    switch (state_)
    {
    case GameState::Paused:
        break;
    case GameState::Running:
        updateGameRunning();
        break;
    case GameState::GameOver:
        updateGameOver();
        break;
    case GameState::Idle:
        updateGameIdle();
        break;
    default:
        break;
    }
    
    return;   
             
}

void Game::updateGameRunning()
{
    
    // Clear the enitites vector;
    entities_.clear();
    
    // update spaceship pose
    spaceship_->updatePose();
    entities_.emplace_back(spaceship_);

    // update level according to score
    level_  = score_ / 1000;;

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

    // Calculate the time since the last asteroid
    auto asteroid_time = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - asteroid_time_);
    // Update the interval according to the game level
    asteroids_interval_ = std::max(INITIAL_INTERVAL - level_, MIN_INTERVAL );

    // create a new asteroid every interval seconds / interval decreases as level rises;
    if (asteroid_time.count() >= asteroids_interval_ ){
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
    
    // Create the game texts;
    createTexts((asteroids_interval_ - asteroid_time.count()));

}

void Game::createTexts(const int &countdown)
{
    // Create the score entity
    auto message = std::make_shared<DrawableEntity>();
    DrawableText message_text;
    std::stringstream  text;
    text  << "SCORE: " << score_ ;
    message_text.text = text.str();
    message_text.color = {255, 255, 255, 0};
    message_text.rect = {550, 0, 180, 60};

    message->addText(std::move(message_text));

    
    // Create the Level entity
    text.str("");
    text.clear();

    text << "Level: " << level_ + 1;//
    message_text.text = text.str();
    message_text.rect = {0, 40, 140, 60};

    message->addText(std::move(message_text));
    
    
    // Create the asteroid timer entity
    text.str("");
    text.clear();

    text << "ASTEROID IN: " << countdown;//
    message_text.text = text.str();
    message_text.rect = {0, 80, 180, 60};

    message->addText(std::move(message_text));
    
    
    // Create the FPS entity
    text.str("");
    text.clear();

    text << "FPS: " << actual_fps_;
    message_text.text = text.str();
    message_text.rect = {1160, 0, 120, 60};

    message->addText(std::move(message_text));
    
    
    // Add the message the the entities vector;
    entities_.emplace_back(message);

}

void Game::updateGameOver()
{
    
    // Clear the enitites vector;
    entities_.clear();

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
    
    return;
}

void Game::updateGameIdle()
{
    
    // Clear the enitites vector;
    entities_.clear();

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

    std::shared_ptr<Missile> missile;

    if (state_ == GameState::Running){
        // a key is pressed
        if (e.type == SDL_KEYDOWN){
            switch (e.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                this->pause();
                state_ = GameState::Paused;
                break;
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
                spaceship_->shoot(missile);
                if (missile){
                    missiles_.emplace_back(missile);
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

    // Return to start again
    else if (state_ == GameState::Paused){
        // a key is pressed
        if (e.type == SDL_KEYDOWN){
            if (e.key.keysym.sym == SDLK_ESCAPE)
            this->resume();
            state_ = GameState::Running;
        }
    }


    return;

}


void Game::createSpaceship()
{
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

}
// Cretae a new astroid on a random place on the top row of the screen, with a random speed
void Game::createAstroid()
{
    auto asteroid = std::make_shared<Asteroid>();
    DrawableRect rect;
    rect.color = {128, 0, 128, 0};
    
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
    
     
    // Iterate through asteroids
    for (auto &asteroid:asteroids_){
        // Check intersection with the spacship
        if (spaceship_->intersects(asteroid)){
            return true;
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
     
    // Iterate through missiles and asteroids 
    for (auto &missile:missiles_){
        for (auto &asteroid:asteroids_){
            // Make sure Asteroid was not already hit;
            if (asteroid->isAlive() == false)
                continue;
            // Check intersection between missile and asteroid
            if (missile->intersects(asteroid)){
                asteroid->explode();
                missile->explode();
                std::cout << "Missile Hit" << std::endl;
                score_ += 100;
            }
        }
    }
  
    return;
}
    
// reset the game variables
void Game::reset()
{
    spaceship_->stop();
    spaceship_->setPose(Pose(WINDOW_WIDTH / 2, WINDOW_HEIGHT - spaceship_->getHeight() / 2));
    missiles_.clear();
    asteroids_.clear();
    
    asteroid_time_ = std::chrono::system_clock::now();
    score_ = 0;
    level_ = 0;
    asteroids_interval_ = INITIAL_INTERVAL;
    createAstroid();
}

void Game::pause()
{
    // Save the the delta between the clock and the last asteroid
    asteroid_delta_ = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - asteroid_time_);
}

void Game::resume()
{
    // Set the last asteroid time, according to the current system clock
    asteroid_time_ = std::chrono::system_clock::now() - asteroid_delta_;
}

