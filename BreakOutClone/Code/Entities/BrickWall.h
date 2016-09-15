#pragma once
#include "Brick.h"
#include <vector>

class BrickWall: public Entity {
public:
	BrickWall(std::vector<Brick*> wall, SDL_Rect wallBoundaries, int destroyableBricks);
	~BrickWall();
	void render(SDL_Renderer* sdlRenderer) override;
	bool intersectsRectangle(Entity* entity) override;
	bool checkForDestroyedBricks();
	int destroyableBricksCount();
protected:
	std::vector<Brick*> bricks_;
	SDL_Rect boundaries_;
	int destroyableBricks_{ 0 };
};
