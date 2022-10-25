#include <iostream>
#include <random>
#include <ctime>
#include <memory>
#include <experimental/filesystem>

#include "renderer.h"


namespace fs = std::experimental::filesystem;
using namespace asteroids;

bool Renderer::init()
{
    // SDL INIT
    if (SDL_Init(SDL_INIT_VIDEO) != 0){
		std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return false;
	}

    // Create a SDL WINDOW
    window_ = SDL_CreateWindow("SDL2 - Asteroids", 100, 100, width_, height_, SDL_WINDOW_SHOWN);
    if (window_ == nullptr){
        std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return false;
    }
    
    // Create an SDL Renderer
    renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer_ == nullptr){
        std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window_);
        SDL_Quit();
        return false;
    }

    TTF_Init();
    auto font_path = fs::current_path().parent_path().string() + "/fonts/FreeSans.ttf";
    font_ = TTF_OpenFont(font_path.c_str(),  48 );

     if (font_ == NULL)
        return false;
    
    this->initiated_ = true;

    return true;
}

Renderer::~Renderer()
{
    std::cout<< "Renderer::~Renderer()" << std::endl;
    if (initiated_){
        SDL_DestroyRenderer(renderer_);
        SDL_DestroyWindow(window_);
        SDL_Quit();
        TTF_Quit();
    }
    
}

void Renderer::render(std::vector<std::shared_ptr<DrawableEntity>> &entities)
{
    
    // Reneder all drawable entities
    for (auto &entity:entities){
        Pose pose = entity->getPose();
        // Render the rects
        for (auto &rect:entity->getRects()){
            SDL_SetRenderDrawColor(renderer_, rect.color.r, rect.color.g, rect.color.b, rect.color.a);
            rect.rect.x += pose.x;
            rect.rect.y += pose.y;
            SDL_RenderFillRect(renderer_, &rect.rect);
            rect.rect.x -= pose.x;
            rect.rect.y -= pose.y;
        }
        // Render the texts
        for (auto &text:entity->getTexts()){
            auto texture = createTextureFromText(text.text, text.color);
            SDL_RenderCopy(renderer_, texture, NULL, &text.rect);
                    
        }
    }
  
    return ;

}

void Renderer::render(std::shared_ptr<DrawableEntity> entity)
{
    if (!entity) return;
    
    auto  pose = entity->getPose();

    for (auto &rect:entity->getRects()){
        SDL_SetRenderDrawColor(renderer_, rect.color.r, rect.color.g, rect.color.b, rect.color.a);
        rect.rect.x += pose.x;
        rect.rect.y += pose.y;
        SDL_RenderFillRect(renderer_, &rect.rect);
        rect.rect.x -= pose.x;
        rect.rect.y -= pose.y;
    }
    // Render the texts
    for (auto &text:entity->getTexts()){
        auto texture = createTextureFromText(text.text, text.color);
        SDL_RenderCopy(renderer_, texture, NULL, &text.rect);
                
    }
    return ;
}

// Clear the screen
void Renderer::clear()
{
     
    SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);
    SDL_RenderClear(renderer_);
    return;
}

// present to screen
void Renderer::present()
{
    //Update the screen
    SDL_RenderPresent(renderer_);
}

SDL_Texture* Renderer::createTextureFromText(std::string text, SDL_Color)
{
   
    SDL_Surface * surface;
    SDL_Color text_color = {255, 255, 255, 0};
    
    surface = TTF_RenderText_Solid(font_, text.c_str(), text_color);

    SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer_, surface);

    SDL_FreeSurface(surface);

    int text_width = surface->w;
    int text_height = surface->h;

    SDL_Rect rect = {640, 360, text_width, text_height};

    return texture;
}

