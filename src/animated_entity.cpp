#include <SDL2/SDL_image.h>
#include <iostream>
#include "animated_entity.h"


using namespace asteroids;

bool AnimatedEntity::loadSpriteSheet(const std::string& image_path,  const int& size)
{
    
    if( renderer_ == NULL ){
		std::cout << "loadSpriteSheet: Renderer is not set: " << image_path<< std::endl;
		return false;
	}
	
    //Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( image_path.c_str() );
	if( loadedSurface == NULL ){
		std::cout << "loadSpriteSheet(): Failed to load image: " << image_path<< std::endl;
		return false;
	}
	else
	{
		
		spritesheet_width = loadedSurface->w ;
		spritesheet_height = loadedSurface->h;
		size_ = size;

		width_ = spritesheet_width / size_;
		height_ = spritesheet_height / size_;


		//Color key image
		SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );

		//Create texture from surface pixels
        spritesheet_ = SDL_CreateTextureFromSurface(renderer_ , loadedSurface );
		SDL_FreeSurface( loadedSurface );
		
		if( spritesheet_ == NULL ){
			std::cout << "loadTexture(): Failed to create surface from texture" << std::endl;
			return false;
		}


	}


	return true;
}

AnimatedEntity::~AnimatedEntity()
{
    SDL_DestroyTexture(spritesheet_);
	spritesheet_ = NULL;
}


void AnimatedEntity::updateAnimationIndex(){
	auto now = std::chrono::system_clock::now();
    auto dt = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_);
    if (dt.count() >= 1000 / animation_fps_){
        animation_index_ = (++animation_index_) % (size_*size_);
        last_ = now;
    }

}

AnimatedEntity::AnimatedEntity() : spritesheet_(NULL), animation_index_(0), size_(1)
{
   last_ = std::chrono::system_clock::now();
}