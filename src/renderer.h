#ifndef SPACE_INVADERS_RENDERER_H
#define SPACE_INVADERS_RENDERER_H

#include <string>
#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "moving_entity.h"


namespace space_invaders
{
    class Renderer{
    public:
        Renderer(int window_width, int window_height);
        Renderer() = delete;
        ~Renderer();
        bool init();
        bool render(std::vector<std::shared_ptr<MovingEntity>> &entities);
        bool render(std::vector<SDL_Rect> &rects);
        void getImageSize(std::string image_path, int &w, int &h);
        bool isInitiated(){return initiated_;}
    private:
        bool initiated_;
        int width_;
        int height_;
        SDL_Window * window_;
        SDL_Renderer * renderer_;
        SDL_Texture* loadTexture(const std::string &file);
        void renderTexture(SDL_Texture *tex, int x, int y);
        void logSDLError(std::ostream &os, const std::string &msg);

    };
}


#endif