// A wrapper class for using SDL2, for renedering and displaying object of type DrawableEntity

#ifndef ASTEROIDS_RENDERER_H
#define ASTEROIDS_RENDERER_H

#include <string>
#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#include "texture_entity.h"
#include "rects_entity.h"
#include "texts_entity.h"
#include "animated_entity.h"
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
        void render(std::shared_ptr<TextureEntity> entity);
        void render(std::shared_ptr<RectsEntity> entity);
        void render(std::shared_ptr<TextsEntity> texts);
        void render(std::shared_ptr<AnimatedEntity> entity);

        void playSFX(const SFX_Type& type);
        void playTheme();
        void pauseTheme();
        void resumeTheme();
        void clear();
        void present();
        SDL_Texture * createTextureFromText(std::string, SDL_Color);
        SDL_Renderer * getRenderer(){ return renderer_; }
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
        Mix_Music * theme_;
    };
}


#endif