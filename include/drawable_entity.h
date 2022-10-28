#ifndef ASTEROIDS_DRAWABLE_ENTITY_H
#define ASTEROIDS_DRAWABLE_ENTITY_H

#include <string>
#include <vector>
#include <utility>
#include <memory>
#include <SDL2/SDL.h>

namespace asteroids
{
    struct Pose{
        Pose(): x(0), y(0), rot(0.0){}
        Pose(int x_, int y_) : x(x_), y(y_), rot(0.0){}
        int x,y;
        float rot;
    };
    struct DrawableRect{
        SDL_Rect rect = {0,0,0,0};
        SDL_Color color {0,0,0,0};
    };

    struct DrawableText{
        SDL_Rect rect = {0,0,0,0};
        SDL_Color color {0,0,0,0};
        std::string text;
    };

    struct Speed{
        int x;
        int y;
        Speed(int x, int y) : x(x), y(y){}
    };

    enum class EntityType{IMAGE, RECTS};
    
    class DrawableEntity{
    public:
        DrawableEntity(){}
        Pose getPose(){ return pose_;}
        void setPose(Pose &&pose){pose_ = pose;}
        int getWidth() { return width_; }
        int getHeight() { return width_; }
        void setWidth(int w) { width_ = w; }
        void setHeight(int h) { height_ = h; }
        std::vector<DrawableRect> getRects() { return rects_; }
        std::vector<DrawableText> getTexts() { return texts_; }
        void clear();
        void addRect(DrawableRect &&rect){ rects_.emplace_back(rect); }
        void addText(DrawableText &&text){ texts_.emplace_back(text); }
        bool intersects(std::shared_ptr< DrawableEntity> entity);

    protected:
        int width_, height_, window_width_, window_height_;
        std::vector<DrawableRect> rects_;
        std::vector<DrawableText> texts_;
        Pose pose_;
    };

}

#endif