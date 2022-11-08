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
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0){
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
    font_ = TTF_OpenFont(font_path.c_str(),  48);

    if (font_ == NULL){
        return false;
    }
    
    // Open the audio channel
    if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 ){
        printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
        return false;
    }

    //Load sound effects
    auto sfx_path = fs::current_path().parent_path().string() + "/sfx/launch.wav";
    launch_sfx_ = Mix_LoadWAV( sfx_path.c_str() );
    if( launch_sfx_ == NULL )
    {
        printf( "Failed to load sfx sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        return false;
    }

    sfx_path = fs::current_path().parent_path().string() + "/sfx/hit.wav";
    hit_sfx_ = Mix_LoadWAV( sfx_path.c_str() );
    if( hit_sfx_ == NULL )
    {
        printf( "Failed to load sfx sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        return false;
    }
    sfx_path = fs::current_path().parent_path().string() + "/sfx/game_over.wav";
    game_over_sfx_ = Mix_LoadWAV( sfx_path.c_str() );
    if( game_over_sfx_ == NULL )
    {
        printf( "Failed to load sfx sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        return false;
    }

    // load background theme
    theme_ = Mix_LoadMUS((fs::current_path().parent_path().string() + "/sfx/theme.wav").c_str());
    if( theme_ == NULL )
    {
        printf( "Failed to load theme music! SDL_mixer Error: %s\n", Mix_GetError() );
        return  false;
    }
    
    this->initiated_ = true;

    return true;
}

Renderer::~Renderer()
{
    if (initiated_){
        SDL_DestroyRenderer(renderer_);
        SDL_DestroyWindow(window_);
        Mix_FreeChunk(launch_sfx_);
        Mix_FreeChunk(hit_sfx_);
        Mix_FreeChunk(game_over_sfx_);
        Mix_FreeMusic( theme_ );
        SDL_Quit();
        TTF_Quit();
    }
    
}

void Renderer::render(std::shared_ptr<TextureEntity> entity)
{
    if (!entity) return;
        
    auto pose = entity->getPose();

    auto texture = entity->getTexture();
        
    if (texture !=NULL){
        SDL_Rect dest;
        dest.x = pose.x;
        dest.y = pose.y;
        dest.w = entity->getWidth();
        dest.h = entity->getHeight();
        SDL_RenderCopy(renderer_, texture, NULL, &dest);
        return;
    }
}
void Renderer::render(std::shared_ptr<AnimatedEntity> entity)
{
    if (!entity) return;
        
    auto pose = entity->getPose();

    auto sprite_sheet = entity->getSpriteSheet();
    auto index = entity->getAnimationIndex();
    auto width = entity->getWidth();
    auto height = entity->getHeight();
    auto size = entity->getSpriteSheetSize();
        
    
    SDL_Rect source;

    source.x = width * (index % size);
    source.y = height * (index / size);
    source.w = width;
    source.h = width;


    if (sprite_sheet !=NULL){
        SDL_Rect dest;
        dest.x = pose.x;
        dest.y = pose.y;
        dest.w = source.w;
        dest.h = source.h;
        SDL_RenderCopy(renderer_, sprite_sheet, &source, &dest);
        return;
    }
}

void Renderer::render(std::shared_ptr<RectsEntity> entity)
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
    
    return ;
}
void Renderer::render(std::shared_ptr<TextsEntity> entity)
{
    if (!entity) return;
        
    // Render the texts
    for (auto &text:entity->getTexts()){
        if (!text.text.empty()){
            auto texture = createTextureFromText(text.text, text.color);
            SDL_RenderCopy(renderer_, texture, NULL, &text.rect);
            SDL_DestroyTexture(texture);
        }
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

void Renderer::playSFX(const SFX_Type& type)
{

     auto vol = Mix_Volume(-1, -1);

    switch (type)
    {
        case SFX_Type::Launch:
            Mix_PlayChannel( -1, launch_sfx_, 0 );
            break;
        case SFX_Type::Hit:
            Mix_Volume(-1, vol / 10);
            Mix_PlayChannel( -1, hit_sfx_, 0 );
            Mix_Volume(-1, vol);
            break;
        case SFX_Type::GameOver:
            Mix_Volume(-1, vol / 10);
            Mix_PlayChannel( -1, game_over_sfx_, 0 );
            Mix_Volume(-1, vol);
            break;
        
        default:
            break;
    }
    
}

void Renderer::playTheme()
{
    if (!Mix_PlayingMusic()){
        Mix_PlayMusic( theme_, -1 );
    }
    else if(Mix_PausedMusic()){
        Mix_ResumeMusic();
    }
}

void Renderer::pauseTheme()
{
    Mix_PauseMusic();
}