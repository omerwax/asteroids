#ifndef ASTEROIDS_ANIMATED_ENTITY_H
#define ASTEROIDS_ANIMATED_ENTITY_H

#include <string>
#include <vector>
#include <memory>
#include <SDL2/SDL.h>
#include <chrono>
#include <texture_entity.h>

namespace asteroids
{
    
 
    class AnimatedEntity : public GameEntity{
    public:
        AnimatedEntity() : spritesheet_(NULL), animation_index_(0){}
        ~AnimatedEntity();
        void clear();
        void update(){};
        SDL_Texture* getSpriteSheet(){ return spritesheet_;}
        int getAnimationIndex(){ return animation_index_;}
        int getSpriteSheetSize(){ return size_;}
   
    protected:
        bool loadSpriteSheet(const std::string& image_path, const int& size);
        SDL_Renderer*  renderer_;
        int animation_rate_= 30;
        int animation_interval_ = 1000 / animation_rate_;
        std::chrono::time_point<std::chrono::system_clock> last_;
        int size_;                

        int animation_index_;
         
    private:
        SDL_Texture * spritesheet_;
        int spritesheet_width;
        int spritesheet_height;
    };

}

#endif