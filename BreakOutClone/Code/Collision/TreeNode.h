#pragma once

#include <SDL.h>
#include <vector>
#include "../Entities/Entity.h"
#include "DoubleRect.h"

class QuadTree;

const int QUAD = 4;
const int MAX_OBJECTS = 4;
const int MAX_LEVELS = 5;

class TreeNode
{
public:
	TreeNode();
	TreeNode(const SDL_Rect &boundaries, QuadTree* owner, int depth, TreeNode* parentNode);
	~TreeNode();
	void insertEntity(Entity* entity);
	bool contains(TreeNode* subQuad, Entity* object);
	SDL_Rect bounds() { return bounds_; }
	void split();
	TreeNode * locateEntityPositionInQuadTree(Entity * entity);
	void checkForDestroyedElements();
	void render(SDL_Renderer* sdlRenderer);
	void debugRender(SDL_Renderer * sdlRenderer, int r, int g, int b, int a);
	std::vector<Entity*> retrieveEntities();
	void clear();

private:
	//structure
	int depth_{ 0 };
	QuadTree* quadTree_{ 0 };
	TreeNode* parentNode_{ 0 };
	std::vector<TreeNode*> subNodes_;
	bool isLeaf_;
	
	//access
	SDL_Rect bounds_{ 0, 0, 0, 0 };
	DoubleRect* boundsNormalized_{ 0 };
	long int hashTag_{ 0 };

	//contents
	std::vector<Entity*> objects_;
};
