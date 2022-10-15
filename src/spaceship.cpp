#include "spaceship.h"
#include "game.h"

using namespace space_invaders;


std::shared_ptr<Missile> Spaceship::shoot()
{
    auto missile = std::make_shared<Missile>();
    DrawableRect rect;
    Pose pose = this->getPose();
    
       
    rect.rect = {getWidth() / 2 - 6, - 20, 6, 20};
    rect.color = {128, 128, 128, 0};
    missile->setWidth(6);
    missile->setHeight(20);
    missile->setPose(std::move(pose));

    missile->addRect(rect);
    missile->setSpeed(Speed(0, -5));

    return missile;

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