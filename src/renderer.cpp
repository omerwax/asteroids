#include <iostream>
#include <random>
#include <ctime>
#include <memory>

#include "renderer.h"



using namespace space_invaders;

Renderer::Renderer(int window_width, int window_height) : 
    width_(window_width), height_(window_height), 
    initiated_(false){}

bool Renderer::init()
{
    // SDL INIT
    if (SDL_Init(SDL_INIT_VIDEO) != 0){
		std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return false;
	}

    // Create a SDL WINDOW
    window_ = SDL_CreateWindow("Space Invaders!!!", 100, 100, width_, height_, SDL_WINDOW_SHOWN);
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

    this->initiated_ = true;

    return true;
}

Renderer::~Renderer()
{
    std::cout<< "Renderer::~Renderer()::Inside" << std::endl;
    if (initiated_){
        SDL_DestroyRenderer(renderer_);
        SDL_DestroyWindow(window_);
        SDL_Quit();
    }
    
}

void Renderer::logSDLError(std::ostream &os, const std::string &msg){
	os << msg << " error: " << SDL_GetError() << std::endl;
}

SDL_Texture* Renderer::loadTexture(const std::string &file){
	//Initialize to nullptr to avoid dangling pointer issues
	SDL_Texture *texture = nullptr;
	//Load the image
	SDL_Surface *loadedImage = SDL_LoadBMP(file.c_str());
	//If the loading went ok, convert to texture and return the texture
	if (loadedImage != nullptr){
		texture = SDL_CreateTextureFromSurface(renderer_, loadedImage);
		SDL_FreeSurface(loadedImage);
		//Make sure converting went ok too
		if (texture == nullptr){
			logSDLError(std::cout, "CreateTextureFromSurface");
		}
	}
	else {
		logSDLError(std::cout, "LoadBMP");
	}
	return texture;
}

void Renderer::renderTexture(SDL_Texture *tex, int x, int y){
	//Setup the destination rectangle to be at the position we want
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	//Query the texture to get its width and height to use
	SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
	SDL_RenderCopy(renderer_, tex, NULL, &dst);
}

bool Renderer::render(std::vector<std::shared_ptr<MovingEntity>> &entities)
{
    SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);
    SDL_RenderClear(renderer_);

    for (auto &entity:entities){
        Pose pose = entity->getPose();
        if (entity->isImageType()){
            SDL_Texture *image = loadTexture(entity->getImagePath());
            if (image == nullptr){
                return false;
            }
            int iW, iH;
            SDL_QueryTexture(image, NULL, NULL, &iW, &iH);

            renderTexture(image, std::min<int>(pose.x, width_ - iW), std::min(pose.y, height_ - iH));
        }
        else{
            for (auto &rect:entity->getRects()){
                SDL_SetRenderDrawColor(renderer_, rect.color.r, rect.color.g, rect.color.b, rect.color.a);
                rect.rect.x += pose.x;
                rect.rect.y += pose.y;
                SDL_RenderFillRect(renderer_, &rect.rect);
                rect.rect.x -= pose.x;
                rect.rect.y -= pose.y;
            }

        }
    }

    //Update the screen
    SDL_RenderPresent(renderer_);

    return true;

}


void Renderer::getImageSize(std::string image_path, int &w, int &h)
{
    SDL_Texture *image = loadTexture(image_path);
    SDL_QueryTexture(image, NULL, NULL, &w, &h);

}




