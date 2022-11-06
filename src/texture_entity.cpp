#include <SDL2/SDL_image.h>
#include <iostream>
#include "texture_entity.h"


using namespace asteroids;

bool TextureEntity::loadTexture(const std::string& image_path){
    
    if( renderer_ == NULL ){
		std::cout << "loadTexture(): Renderer is not set: " << image_path<< std::endl;
		return false;
	}
	
	texture_ = NULL;

    //Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( image_path.c_str() );
	if( loadedSurface == NULL ){
		std::cout << "loadTexture(): Failed to load image: " << image_path<< std::endl;
		return false;
	}
	else
	{
		
		width_ = loadedSurface->w;
		height_ = loadedSurface->h;

		//Color key image
		SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );

		//Create texture from surface pixels
        texture_ = SDL_CreateTextureFromSurface(renderer_ , loadedSurface );
		
		SDL_FreeSurface( loadedSurface );
		
		if( texture_ == NULL ){
			std::cout << "loadTexture(): Failed to create surface from texture" << std::endl;
			return false;
		}
		
	}

	return true;
}

TextureEntity::~TextureEntity()
{
    SDL_DestroyTexture(texture_);
    texture_ = NULL;
}