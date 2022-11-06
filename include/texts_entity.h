#ifndef ASTEROIDS_TEXTS_ENTITY_H
#define ASTEROIDS_TEXTS_ENTITY_H

#include <string>
#include <vector>
#include <utility>
#include <memory>
#include <SDL2/SDL.h>

namespace asteroids
{
    
    struct DrawableText{
        SDL_Rect rect = {0,0,0,0};
        SDL_Color color {0,0,0,0};
        std::string text;
    };
 
    class TextsEntity {
    public:
              
        std::vector<DrawableText> getTexts() { return texts_; }
        void addText(DrawableText &&text){ texts_.emplace_back(text); }
        void clear(){ texts_.clear(); }
        

    protected:
        std::vector<DrawableText> texts_;
    };

}

#endif