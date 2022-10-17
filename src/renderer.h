#ifndef ASTEROIDS_RENDERER_H
#define ASTEROIDS_RENDERER_H

#include <string>
#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "drawable_entity.h"


namespace asteroids
{
    class Renderer{
    public:
        Renderer(int window_width, int window_height) : 
                width_(window_width), height_(window_height), 
                initiated_(false){}
        Renderer() = delete;
        ~Renderer();
        bool init();
        void render(std::vector<std::shared_ptr<DrawableEntity>> &entities);
        SDL_Texture * createTextureFromText(std::string, SDL_Color);
    private:
        bool initiated_;
        int width_;
        int height_;
        SDL_Window * window_;
        SDL_Renderer * renderer_;
        TTF_Font * font_;
    };
}


#endif