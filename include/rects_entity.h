#ifndef ASTEROIDS_RECTS_ENTITY_H
#define ASTEROIDS_RECTS_ENTITY_H

#include <string>
#include <vector>
#include <utility>
#include <memory>
#include <SDL2/SDL.h>
#include "game_entity.h"

namespace asteroids
{
   
    struct DrawableRect{
        SDL_Rect rect = {0,0,0,0};
        SDL_Color color {0,0,0,0};
    };

    
    class RectsEntity : public GameEntity{
    public:
       
        std::vector<DrawableRect> getRects() { return rects_; }
        void clear(){ rects_.clear(); }
        void createRects(std::vector<DrawableRect> &&rects);
        
    protected:
        std::vector<DrawableRect> rects_;
    };

}

#endif