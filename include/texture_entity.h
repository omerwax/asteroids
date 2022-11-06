#ifndef ASTEROIDS_TEXTURE_ENTITY_H
#define ASTEROIDS_TEXTURE_ENTITY_H

#include <string>
#include <vector>
#include <utility>
#include <memory>
#include <SDL2/SDL.h>
#include <game_entity.h>

namespace asteroids
{
    
 
    class TextureEntity : public GameEntity{
    public:
        TextureEntity() : texture_(NULL), renderer_(NULL){}
        ~TextureEntity();
        int getWidth() { return width_; }
        int getHeight() { return height_; }
        void clear();
        void update(){};
        SDL_Texture* getTexture(){ return texture_;}
        void loadRenderer(SDL_Renderer* renderer) { renderer_ = renderer;}
        bool loadTexture(const std::string& image_path, const std::vector<SDL_Rect>& collisions);
    
    protected:
        SDL_Renderer*  renderer_;
         
    private:
        SDL_Texture * texture_;
        
        
    };

}

#endif