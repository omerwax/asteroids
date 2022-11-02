#include "spaceship.h"
#include "game.h"

using namespace asteroids;


Spaceship::Spaceship(const int& pose_x, const int& pose_y)
{
    max_speed_ = 6; accel_ = 3;
    shot_time_ = std::chrono::system_clock::now();

    pose_.x = pose_x; pose_.y = pose_y;

    // Define spaceship shape by a group of rectangles
    DrawableRect rect;
    
    // Left launcher
    rect.rect = {40, 0, 20, 40};
    rect.color = {255, 0, 0, 0};
    rects_.emplace_back(rect);

    // Right launcher
    rect.rect = {100, 0, 20, 40};
    rect.color = {255, 0, 0, 0};
    rects_.emplace_back(rect);

    // Launchers base
    rect.rect = {20, 40, 120, 20};
    rect.color = {255, 255, 0, 0};
    rects_.emplace_back(rect);

    // Body
    rect.rect = {0, 60, 160, 40};
    rect.color = {255, 127, 0, 0};
    rects_.emplace_back(rect);

    // Left engine
    rect.rect = {20, 100, 40, 10};
    rect.color = {255, 0, 0, 0};
    rects_.emplace_back(rect);

    // Right engine
    rect.rect = {100, 100, 40, 10};
    rect.color = {255, 0, 0, 0};
    rects_.emplace_back(rect);


    // set initial pose
    width_ = 160;
    height_= 110;
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

    // calculate the x pose accroding to the launcher side
    if (launcher == Launcher::Left){
        pose.x += 45;
    }
    else{
        pose.x += 105;
    }    
    
    missile = std::make_shared<Missile>(pose.x, pose.y);

    if (launcher == Launcher::Right){
        shot_time_ = std::chrono::system_clock::now();
    }
    
    return ;

}

void Spaceship::updatePose()
{
    
    pose_.x += speed_.x;
    pose_.x = std::min(pose_.x, WINDOW_WIDTH - width_);
    pose_.x = std::max(pose_.x, 0);

    pose_.y += speed_.y;
    pose_.y = std::min(pose_.y, WINDOW_HEIGHT - height_);
    pose_.y = std::max(pose_.y, 0);

}