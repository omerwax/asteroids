#include "rects_entity.h"


using namespace asteroids;


// Update collisions and 
void RectsEntity::createRects(std::vector<DrawableRect> &&rects)
{
	
	collisions_.clear();
	rects_ = rects;

	//calculate dimenstion from rects
	SDL_Rect union_rect = rects_.front().rect;
	for (const auto& rect:rects_){
		collisions_.emplace_back(rect.rect);
		SDL_UnionRect(&(rect.rect), &union_rect, &union_rect);
	}

	width_=  union_rect.w;
	height_ = union_rect.h;

}

