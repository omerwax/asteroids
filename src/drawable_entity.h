#ifndef SPACE_INVADERS_DRAWABLE_ENTITY_H
#define SPACE_INVADERS_DRAWABLE_ENTITY_H

#include <string>
#include <vector>
#include <utility>
#include <SDL2/SDL.h>

namespace space_invaders
{
    struct Pose{
        Pose(): x(0), y(0), rot(0.0){}
        int x,y;
        float rot;
    };
    struct DrawableRect{
        SDL_Rect rect = {0,0,0,0};
        SDL_Color color {0,0,0,0};
    };

    struct Speed{
        int x;
        int y;
        Speed(int x, int y) : x(x), y(y){}
    };

    enum class EntityType{IMAGE, RECTS};
    
    class DrawableEntity{
    public:
        DrawableEntity() : image_path_(std::string()), image_type_(false){}
        DrawableEntity(std::string image_path) : image_path_(image_path), image_type_(false){}
        void setImage(std::string image_path);
        std::string getImagePath(){ return image_path_; }
        Pose getPose(){ return pose_;}
        void setPose(Pose &&pose){pose_ = pose;}
        int getWidth() { return width_; }
        int getHeight() { return width_; }
        void setWidth(int w) { width_ = w; }
        void setHeight(int h) { height_ = h; }
        bool isImageType()  { return image_type_; }
        std::vector<DrawableRect> getRects() { return rects_; }
        void addRect(DrawableRect rect){ rects_.emplace_back(rect); }

    protected:
        int width_, height_, window_width_, window_height_;
        bool image_type_;
        std::string image_path_;
        std::vector<DrawableRect> rects_;
        Pose pose_;
    };

}

#endif