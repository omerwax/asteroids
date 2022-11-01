// A wrapper class for using SDL2, for renedering and displaying object of type DrawableEntity

#ifndef ASTEROIDS_RENDERER_H
#define ASTEROIDS_RENDERER_H

#include <string>
#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include "drawable_entity.h"
#include "spaceship.h"


namespace asteroids
{
    enum class SFX_Type{Launch, Hit, GameOver};
    
    class Renderer{
    public:
        Renderer(int window_width, int window_height) : 
                width_(window_width), height_(window_height), 
                initiated_(false){}
        Renderer() = delete;
        ~Renderer();
        bool init();
        void render(std::vector<std::shared_ptr<DrawableEntity>> &entities);
        void render(std::shared_ptr<DrawableEntity> entity);
        void playSFX(const SFX_Type& type);
        void clear();
        void present();
        SDL_Texture * createTextureFromText(std::string, SDL_Color);
    private:
        bool initiated_;
        int width_;
        int height_;

        SDL_Window * window_;
        SDL_Renderer * renderer_;
        TTF_Font * font_;
        Mix_Chunk * launch_sfx_;
        Mix_Chunk * hit_sfx_;
        Mix_Chunk * game_over_sfx_;
    };
}


#endif