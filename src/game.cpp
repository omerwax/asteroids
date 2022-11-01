
#include <thread>
#include <chrono>
#include <iostream>
#include <sstream>
#include <fstream>
#include <experimental/filesystem>

#include "game.h"
#include "spaceship.h"
#include "macaron_base64.h"

using namespace asteroids;
namespace fs = std::experimental::filesystem;

bool Game::init()
{
    
    // Initialize the renderer
    if (!renderer_.init())
        return false;
        
    // Initialize the random engine
    mt_ = std::make_shared<std::mt19937>(std::chrono::system_clock::now().time_since_epoch().count());
    
    texts_ = std::make_shared<DrawableEntity>();
    
    // Open the data file 
    auto data_file = std::fstream("../data/data");
    // create a new file and open if it doesn't exist already
    if (!data_file.is_open()){
        // check if the directory exists
        if (!fs::exists("../data")){
            fs::create_directory("../data");
        }
        auto temp = std::ofstream("../data/data");
        temp.close();
        data_file = std::fstream("../data/data");

    }

    // read the high_Score and player data
    std::string data_b64, data;
    std::getline(data_file, data_b64);
    if (data_b64.empty()){
        high_score_ = 0;
    }
    else{
        macaron::Base64::Decode(data_b64, data);
        std::istringstream input(data); 
        input >> high_score_ ;
        input >> high_score_player_;
    }

    data_file.close();

    createSpaceship();
    
    state_ = GameState::Idle;

    return true;
}

void Game::render()
{
    // clear the screen
    renderer_.clear();
    
    if (state_ == GameState::Running || state_ == GameState::Paused || state_ == GameState::GameOver){
        // render the spaceship
        renderer_.render(spaceship_);

        // render the asterids
        for (auto &astertoid:asteroids_)
            renderer_.render(astertoid);
        
        // render te missiles
        for (auto &missile:missiles_)
             renderer_.render(missile);
    }
    
    // render the texts;
    renderer_.render(texts_);
    
    // present
    renderer_.present();
}

void Game::run()
{
    int frame = 0;

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
       
    }

    return;
    
}

// Update function update the position of all the game entities, check collisions and missile hits
// In addition it populates the entities_ vector with entities for rendering. It also 
void Game::update()
{
    // Process events 
    processEvents();   
    
    switch (state_)
    {
    case GameState::Paused:
        break;
    case GameState::PlayerName:
        updateGamePlayerName();
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
void Game::updateGamePlayerName()
{
    texts_->clear();
    
    DrawableText message_text;
    
    message_text.text = "PLEASE ENTER YOUR NAME";
    message_text.color = {255, 255, 255, 0};
    int rect_size_x = (int)message_text.text.length()*24;
    int rect_pose_x = WINDOW_WIDTH / 2 - rect_size_x / 2;
    message_text.rect = {rect_pose_x, 0, rect_size_x, 60};

    texts_->addText(std::move(message_text));

    message_text.text = player_name_;
    message_text.color = {255, 255, 255, 0};
    rect_size_x = (int)player_name_.length()*24;
    rect_pose_x = WINDOW_WIDTH / 2 - rect_size_x / 2;
    message_text.rect = {rect_pose_x, 330, rect_size_x, 60};

    texts_->addText(std::move(message_text));

  
    return;

}

void Game::updateGameRunning()
{
    
    
    // update spaceship pose
    spaceship_->updatePose();
    
    // update level according to score
    level_  = score_ / 1000;;

    // Update all missile, delete ones that are out of scope(screen)
    auto it = missiles_.begin();
    while ( it != missiles_.end()){
        (*it)->updatePose();
        // check if missile is out of screen
        if ((*it)->isAlive() == false){
            // remove it from the missiles_ vector (also deallocate it as it is no longer owned by anyone)
            missiles_.erase(it);
        }
        else it++;
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
        if ((*it_s)->isAlive() == false){
            // remove it from the missiles_ vector (also deallocate it as it is no longer owned by anyone)
            asteroids_.erase(it_s);
        }
        else it_s++;
    }

    // Check if the spaceship collided with an asteroid
    if (checkCollisions()){
        std::cout << "Spaceship is hit by an asteroid\nGAME - OVER!!!!" << std::endl;
        final_score_ = score_;
        prev_state_ = state_;
        state_ = GameState::GameOver;
    }

    // Check for missile hits
    checkHits();
    
    // Create the game texts;
    createTexts((asteroids_interval_ - asteroid_time.count()));

}

// Create the GameRunning state texts
void Game::createTexts(const int &countdown)
{
    
    // clear previous texts
    texts_->clear();
    
    // Create the score entity
    DrawableText message_text;
    std::stringstream  text;
    text  << "SCORE: " << score_ ;
    message_text.text = text.str();
    message_text.color = {255, 255, 255, 0};
    int rect_size_x = (int)message_text.text.length()*24;
    int rect_pose_x = WINDOW_WIDTH / 2 - rect_size_x / 2;
    message_text.rect = {rect_pose_x, 0, rect_size_x, 60};

    texts_->addText(std::move(message_text));

    // Create the Name entity
    message_text.text = player_name_;
    rect_size_x = (int)message_text.text.length()*24;
    rect_size_x = std::min(rect_size_x, WINDOW_WIDTH / 4 );
    message_text.rect = {0, 0, rect_size_x, 60};
    texts_->addText(std::move(message_text));
    
    
    // Create the Level entity
    text.str("");
    text.clear();

    text << "LEVEL: " << level_ + 1;//
    message_text.text = text.str();
    rect_size_x = (int)message_text.text.length()*24;
    message_text.rect = {0, 40, rect_size_x, 60};

    texts_->addText(std::move(message_text));
    
    
    // Create the asteroid timer entity
    text.str("");
    text.clear();

    text << "ASTEROID IN: " << countdown;//
    message_text.text = text.str();
    rect_size_x = (int)message_text.text.length()*24;
    message_text.rect = {0, 80, rect_size_x, 60};

    texts_->addText(std::move(message_text));

    // Create the High score entity
    if (high_score_ > 0){
        text.str("");
        text.clear();

        text << "HIGH SCORE: " << high_score_;//
        message_text.text = text.str();
        rect_size_x = (int)message_text.text.length()*24;
        message_text.rect = {0, 120, rect_size_x, 60};

        texts_->addText(std::move(message_text));

        // Create the High score name entity
        text.str("");
        text.clear();

        text << high_score_player_;//
        message_text.text = text.str();
        rect_size_x = (int)message_text.text.length()*24;
        message_text.rect = {0, 160, rect_size_x, 60};

        texts_->addText(std::move(message_text));
    }
    
    // Create the FPS entity
    text.str("");
    text.clear();

    text << "FPS: " << actual_fps_;
    message_text.text = text.str();
    rect_size_x = (int)message_text.text.length()*24;
    rect_pose_x = WINDOW_WIDTH - rect_size_x;
    message_text.rect = {rect_pose_x, 0, rect_size_x, 60};

    texts_->addText(std::move(message_text));

    return;
     
}

void Game::updateGameOver()
{
    // For static states, do not update more than once
    if (prev_state_ == state_)
        return;
    texts_->clear();
        
    DrawableText message_text;
    std::stringstream  text;
    text  << "Press the ENTER key to play again";
    message_text.text = text.str();
    message_text.color = {255, 255, 255, 0};
    message_text.rect = {WINDOW_WIDTH / 4 , WINDOW_HEIGHT - 80, WINDOW_WIDTH / 2, 60};
    texts_->addText(std::move(message_text));

    text.str("");
    text << "GAME OVER!!!";
    message_text.text = text.str();
    message_text.rect = {WINDOW_WIDTH * 3 / 8 ,0, WINDOW_WIDTH / 4, 60};
            
    texts_->addText(std::move(message_text));

    
    text.str("");
    if (final_score_ > high_score_){
        text << "NEW HIGH SCORE!!!: " << final_score_;
        message_text.text = text.str();
        message_text.rect = {WINDOW_WIDTH * 3 / 8 , 80, WINDOW_WIDTH / 4, 60};
        high_score_ = final_score_;
        high_score_player_ = player_name_;
        texts_->addText(std::move(message_text));
        // Update the highscore data file
        updateHighScore();
    }
    else{
        text << "SCORE: " << final_score_;
        message_text.text = text.str();
        message_text.rect = {WINDOW_WIDTH * 3 / 8 , 80, WINDOW_WIDTH / 4, 60};
                
        texts_->addText(std::move(message_text));
    }

    prev_state_ = state_;
    return;
}

void Game::updateGameIdle()
{
    
    // For static states, do not update more than once
    if (prev_state_ == state_)
        return;
    texts_->clear();

    DrawableText message_text;
    std::stringstream  text;
    text  << "Asteroids!!! Press space key to start shooting";
    message_text.text = text.str();
    message_text.color = {255, 255, 255, 0};
    message_text.rect = {WINDOW_WIDTH / 2 - WINDOW_WIDTH / 4 , 0, WINDOW_WIDTH / 2, 60};

    texts_->addText(std::move(message_text));
    
    prev_state_ = state_;

    return;

}
// Processing user input according to game state
void Game::processInput(){
    //Handle events on queue
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0){
        // First check for quit event
        if (e.type == SDL_QUIT){
            events_.emplace_back(GameEvent(EventType::End));
            return;
        }
        if (e.type == SDL_KEYDOWN){
            switch (state_){
                case GameState::GameOver:
                    if (e.key.keysym.sym == SDLK_RETURN){
                        events_.emplace_back(GameEvent(EventType::Restart));
                    }
                    if (e.key.keysym.sym == SDLK_q){ 
                        events_.emplace_back(GameEvent(EventType::End));
                        return;
                    }
                    break;
                case GameState::Paused:
                    if(e.key.keysym.sym == SDLK_ESCAPE){
                        events_.emplace_back(GameEvent(EventType::Resume));
                    }
                    break;
                case GameState::Idle:
                    if(e.key.keysym.sym == SDLK_SPACE){
                        events_.emplace_back(GameEvent(EventType::Name));
                    }
                    break;
                case GameState::PlayerName:
                        events_.emplace_back(GameEvent(EventType::KeyTyped, e.key.keysym.sym));
                    break;
                case GameState::Running:
                    if(e.key.keysym.sym == SDLK_ESCAPE){
                        events_.emplace_back(GameEvent(EventType::Pause));
                    }
                    if(e.key.keysym.sym == SDLK_SPACE){
                        events_.emplace_back(GameEvent(EventType::Shoot));
                    }
                    if(e.key.keysym.sym == SDLK_RIGHT){
                        events_.emplace_back(GameEvent(EventType::AccelRight));
                    }
                    if(e.key.keysym.sym == SDLK_LEFT){
                        events_.emplace_back(GameEvent(EventType::AccelLeft));
                    }
                    if(e.key.keysym.sym == SDLK_UP){
                        events_.emplace_back(GameEvent(EventType::AccelUp));
                    }
                    if(e.key.keysym.sym == SDLK_DOWN){
                        events_.emplace_back(GameEvent(EventType::AccelDown));
                    }
                    break;

                default:
                    break;
            }
        }      
    }
    return;
}

void Game::processEvents(){

    while (events_.size() > 0){
        
        auto event = events_.front();
        events_.erase(events_.begin());
        auto e_type = event.type();
        
        if (e_type == EventType::End){
            state_ = GameState::End;
            return;
        }

        std::shared_ptr<Missile> missile_l;
        std::shared_ptr<Missile> missile_r;

        auto key = event.key();
      
        switch(e_type){
            case EventType::Pause:
                this->pause();
                prev_state_ = state_;
                state_ = GameState::Paused;
                break;
            case EventType::Resume:
                this->resume();
                prev_state_ = state_;
                state_ = GameState::Running;
                break;
            case EventType::Restart:
                prev_state_ = state_;
                state_ = GameState::PlayerName;
                break;
            case EventType::AccelRight:
                spaceship_->accelRight();
                break;
            case EventType::AccelLeft:
                spaceship_->accelLeft();
                break;
            case EventType::AccelUp:
                spaceship_->accelUp();
                break;
            case EventType::AccelDown:
                spaceship_->accelDown();
                break;
            case EventType::Shoot:
                spaceship_->shoot(missile_l, Launcher::Left);
                spaceship_->shoot(missile_r, Launcher::Right);
                if (missile_l && missile_r){
                    missiles_.emplace_back(missile_l);
                    missiles_.emplace_back(missile_r);
                    renderer_.playSFX(SFX_Type::Launch);
                }
                break;
            case EventType::Hit:
                renderer_.playSFX(SFX_Type::Hit);
                break;
            case EventType::Name:
                state_ = GameState::PlayerName;
                break;
            case EventType::KeyTyped:
                if ((key >= SDLK_a && key <= SDLK_z)){
                    player_name_+= char(key - 32);
                }
                if ((key == SDLK_SPACE )){
                    player_name_ += char(SDLK_SPACE);
                }
                else if(key == SDLK_BACKSPACE){
                    if (player_name_.size() > 0) player_name_.resize(player_name_.size() - 1);
                }
                else if (key == SDLK_RETURN)
                {
                    prev_state_ = state_;
                    state_ = GameState::Running;
                    this->reset();
                }
                break;
            default:
                break;
        }
       
    }   
   
}

void Game::createSpaceship()
{
    // Define spaceship shape by a group of rectangles
    DrawableRect rect;
    spaceship_ = std::make_shared<Spaceship>();
    
    // Left launcher
    rect.rect = {40, 0, 20, 40};
    rect.color = {255, 0, 0, 0};
    spaceship_->addRect(std::move(rect));

    // Right launcher
    rect.rect = {100, 0, 20, 40};
    rect.color = {255, 0, 0, 0};
    spaceship_->addRect(std::move(rect));

    // Launchers base
    rect.rect = {20, 40, 120, 20};
    rect.color = {255, 255, 0, 0};
    spaceship_->addRect(std::move(rect));

    // Body
    rect.rect = {0, 60, 160, 40};
    rect.color = {255, 127, 0, 0};
    spaceship_->addRect(std::move(rect));

    // Left engine
    rect.rect = {20, 100, 40, 10};
    rect.color = {255, 0, 0, 0};
    spaceship_->addRect(std::move(rect));

    // Right engine
    rect.rect = {100, 100, 40, 10};
    rect.color = {255, 0, 0, 0};
    spaceship_->addRect(std::move(rect));


    // set initial pose
    spaceship_->setWidth(160);
    spaceship_->setHeight(110);

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
            renderer_.playSFX(SFX_Type::GameOver);
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
                events_.emplace_back(EventType::Hit);
                asteroid->explode();
                missile->explode();
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
// Writes the high score value to the data file
void Game::updateHighScore()
{
    auto data_file = std::ofstream("../data/data");
    if (!data_file.is_open()){
        return;
    }
    
    std::stringstream data;
    data << high_score_ << " " << high_score_player_;
    
    data_file << macaron::Base64::Encode(data.str());
    data_file.close();
}