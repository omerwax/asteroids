#include "spaceship.h"
#include "game.h"

using namespace asteroids;


Spaceship::Spaceship()
{
    max_speed_ = 6; accel_ = 3;
    shot_time_ = std::chrono::system_clock::now();
}


void Spaceship::shoot(std::shared_ptr<Missile> &missile, const Launcher& launcher)
{
    
    auto shots_interval = std::chrono::duration_cast<std::chrono::milliseconds>
                (std::chrono::system_clock::now() - shot_time_);
           
    if(shots_interval.count() <= MIN_SHOOTING_SPEED_MS)
    {
        return;
    }

    missile = std::make_shared<Missile>();

    DrawableRect rect;
    // get the spaceship pose set missile pose to the spaceship pose
    Pose pose = this->pose_;

    // calculate the x pose accroding to the launcher side
    int pose_x = 0;
    if (launcher == Launcher::Left){
        pose_x = 45;
    }
    else{
        pose_x = 105;
    }    
    
    // define the missile rectangle
    rect.rect = {pose_x, -20, 10, 20};
    rect.color = {128, 128, 0, 0}; 
    missile->setWidth(10);
    missile->setHeight(20);
    missile->setPose(std::move(pose));

    missile->addRect(std::move(rect));
    missile->setSpeed(Speed(0, -5));

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