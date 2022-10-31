#ifndef ASTEROIDS_GAME_EVENT_H
#define ASTEROIDS_GAME_EVENT_H

#include <SDL2/SDL.h>

namespace asteroids{

    enum class EventType{Name, Start, Pause, Resume, Shoot, AccelRight, 
                        AccelLeft, AccelUp, AccelDown, End, Restart, KeyTyped, None};
    
    class GameEvent{
    public:
        GameEvent() : type_(EventType::None), key_(SDLK_F19) {};
        GameEvent(const EventType& type) : type_(type), key_(SDLK_F19){};
        GameEvent(const EventType& type, const SDL_Keycode& key) : type_(type), key_(key){};
        EventType type() { return type_ ;}
        SDL_Keycode key(){ return key_; }
    protected:
        EventType type_;
        SDL_Keycode key_;
    };
}

#endif