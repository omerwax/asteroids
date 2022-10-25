#include "drawable_entity.h"

using namespace asteroids;


// Check intersection between two drawable entities
bool DrawableEntity::intersects(std::shared_ptr<DrawableEntity> entity)
{
    for (auto rect1:rects_){
        rect1.rect.x += pose_.x;
        rect1.rect.y += pose_.y;
        for (auto rect2:entity->rects_){
            rect2.rect.x += entity->pose_.x;
            rect2.rect.y += entity->pose_.y;
            if (SDL_HasIntersection(&(rect1.rect),&(rect2.rect))){
                return true; 
            }
        }
    }
    return false;
}

void DrawableEntity::clear()
{
    texts_.clear();
    rects_.clear();
}