#ifndef SPACE_INVADERS_DRAWABLE_ENTITY_H
#define SPACE_INVADERS_DRAWABLE_ENTITY_H

#include <string>
#include <vector>
#include <SDL2/SDL.h>

namespace space_invaders
{
    struct Pose{
        Pose(): x(0), y(0), rot(0.0){}
        int x,y;
        float rot;
    };
    
    class DrawableEntity{
    public:
        DrawableEntity() : image_path_(std::string()), draw_image_(false){}
        DrawableEntity(std::string image_path) : image_path_(image_path), draw_image_(false){}
        void setImage(std::string image_path);
        std::string getImagePath(){ return image_path_; }
        Pose getPose(){ return pose_;}
        void setPose(Pose &&pose){pose_ = pose;}
        int getWidth() { return width_; }
        int getHeight() { return width_; }
        void setWidth(int w) { width_ = w; }
        void setHeight(int h) { height_ = h; }

    private:
        int width_, height_;
        bool draw_image_;
        std::string image_path_;
        Pose pose_;
    };

}

#endif