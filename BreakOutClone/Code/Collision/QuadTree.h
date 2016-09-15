#pragma once

#include "TreeNode.h"
#include "..\Entities\Brick.h"

class QuadTree
{
public:
	QuadTree();
	QuadTree(const SDL_Rect& bounds, int maxDepth);
	~QuadTree();
	int width();
	int height();
	SDL_Rect* bounds();
	void insertBricks(std::vector<Brick*> objects);
	void insertEntity(Entity* entity);
	TreeNode * locateEntityPositionInQuadTree(Entity * entity);
	void checkForDestroyedElements();
	int maxDepth() { return maxDepth_; }
	void render(SDL_Renderer* sdlRenderer);
	void clear();
private:
	SDL_Rect* bounds_{ 0 };
	TreeNode* root_{ 0 };
	int maxDepth_{ 0 };
	int depth_{ 0 };
};
