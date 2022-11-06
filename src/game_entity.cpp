#include <SDL2/SDL_image.h>
#include "game_entity.h"


using namespace asteroids;


// Check intersection between two drawable entities
bool GameEntity::intersects(std::shared_ptr<GameEntity> entity)
{
    for (auto rect1:collisions_){
        rect1.x += pose_.x;
        rect1.y += pose_.y;
        for (auto rect2:entity->collisions_){
            rect2.x += entity->pose_.x;
            rect2.y += entity->pose_.y;
            if (SDL_HasIntersection(&rect1,&rect2)){
                return true; 
            }
        }
    }
    
    return false;
    
}