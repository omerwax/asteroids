#include "spaceship.h"
#include "game.h"

using namespace asteroids;


Spaceship::Spaceship()
{
    max_speed_ = 6; accel_ = 3;
    shot_time_ = std::chrono::system_clock::now();
}

void Spaceship::shoot(std::shared_ptr<Missile> &missile)
{
    
    auto shots_interval = std::chrono::duration_cast<std::chrono::milliseconds>
                (std::chrono::system_clock::now() - shot_time_);
           
    if(shots_interval.count() <= MIN_SHOOTING_SPEED_MS)
    {
        return;
    }

    missile = std::make_shared<Missile>();

    DrawableRect rect;
    Pose pose = this->getPose();
           
    rect.rect = {getWidth() / 2 - 6, - 20, 6, 20};
    rect.color = {128, 0, 0, 0}; 
    missile->setWidth(6);
    missile->setHeight(20);
    missile->setPose(std::move(pose));

    missile->addRect(std::move(rect));
    missile->setSpeed(Speed(0, -5));

    shot_time_ = std::chrono::system_clock::now();
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