#include "missile.h"
#include "game.h"

using namespace asteroids;

void Missile::updatePose()
{
        pose_.x += speed_.x;
    pose_.y += speed_.y;

    if (pose_.y < 0)
        alive_ = false;
}