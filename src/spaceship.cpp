#include "spaceship.h"
#include "game.h"

using namespace asteroids;


Spaceship::Spaceship(SDL_Renderer* renderer)
{
    max_speed_ = 6; accel_ = 3;
    shot_time_ = std::chrono::system_clock::now();

    pose_ = Pose();
    speed_= Speed();

    // Define spaceship collisions
    SDL_Rect rect;

    std::vector<SDL_Rect> collisions;
    
    // Left launcher
    rect = {4, 65, 8, 45};
    collisions.emplace_back(rect);

    // Right launcher
    rect = {130, 65, 8, 45};
    collisions.emplace_back(rect);

    // Cockpit base
    rect = {70, 50, 16, 60};
    collisions.emplace_back(rect);

    // Cockpit top
    rect = {72, 0, 8, 50};
    collisions.emplace_back(rect);

    // Body
    rect = {4, 110, 140, 40};
    collisions.emplace_back(rect);

    // Left engine
    rect = {60, 150, 8, 15};
    collisions.emplace_back(rect);

    // Right engine
    rect = {95, 150, 8, 15};
    collisions.emplace_back(rect);

    loadRenderer(renderer);

    loadTexture("../img/spaceship.png", collisions);

}


void Spaceship::shoot(std::shared_ptr<Missile> &missile, const Launcher& launcher)
{
    
    auto shots_interval = std::chrono::duration_cast<std::chrono::milliseconds>
                (std::chrono::system_clock::now() - shot_time_);
           
    if(shots_interval.count() <= MIN_SHOOTING_SPEED_MS)
    {
        return;
    }

    
    // get the spaceship pose set missile pose to the spaceship pose
    Pose pose = this->pose_;

    
    pose.y += 65;
    // calculate the x pose accroding to the launcher side
    if (launcher == Launcher::Left){
        pose.x += 8; //45;
    }
    else{
        pose.x += 134;//105;
    }    
    
    missile = std::make_shared<Missile>(pose);

    if (launcher == Launcher::Right){
        shot_time_ = std::chrono::system_clock::now();
    }
    
    return ;

}

void Spaceship::update()
{
    
    pose_.x += speed_.x;
    pose_.x = std::min(pose_.x, WINDOW_WIDTH - width_);
    pose_.x = std::max(pose_.x, 0);

    pose_.y += speed_.y;
    pose_.y = std::min(pose_.y, WINDOW_HEIGHT - height_ + 30);
    pose_.y = std::max(pose_.y, 0);

}