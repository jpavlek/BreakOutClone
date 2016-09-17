#pragma once
#include "BrickWall.h"

//-------------------------------------------------------------------------------------------
BrickWall::BrickWall(std::vector<Brick*> wall, SDL_Rect wallBoundaries, int destroyableBricks):
	Entity("BrickWall", wallBoundaries.x, wallBoundaries.y, wallBoundaries.w, wallBoundaries.h),
	bricks_(wall),
	boundaries_(wallBoundaries),
	destroyableBricks_(destroyableBricks)
{
}
//-------------------------------------------------------------------------------------------
BrickWall::~BrickWall()
{
}
//-------------------------------------------------------------------------------------------
void
BrickWall::render(SDL_Renderer* sdlRenderer)
{
	for (size_t i = 0; i < bricks_.size(); i++) {
		bricks_[i]->render(sdlRenderer);
	}
}
//-------------------------------------------------------------------------------------------
bool
BrickWall::intersectsRectangle(Entity* entity)
{
	bool notIntersectsOnX = (boundaries_.x > entity->posX() + entity->width()) || (entity->posX() > boundaries_.x + boundaries_.w);
	bool notIntersectsOnY = (boundaries_.y > entity->posY() + entity->height()) || (entity->posY() > boundaries_.y + boundaries_.h);
	bool intersects = !(notIntersectsOnX || notIntersectsOnY);
	return intersects;
}
//-------------------------------------------------------------------------------------------
bool
BrickWall::checkForDestroyedBricks()
{
	bool erased = false;
	for (int i = bricks_.size() - 1; i > 0; i--) {
		if (bricks_[i]->pointsLeft() <= 0 && bricks_[i]->id() != 'I') {
			destroyableBricks_--;
			bricks_.erase(bricks_.begin() + i);
			erased = true;
		}
	}
	return erased;
}
//-------------------------------------------------------------------------------------------
int
BrickWall::destroyableBricksCount()
{
	return destroyableBricks_;
}
//-------------------------------------------------------------------------------------------