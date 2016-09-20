#include "QuadTree.h"
#include "..\Entities\Entity.h"

//-------------------------------------------------------------------------------------------
QuadTree::QuadTree():
	root_( 0 )
{
}
//-------------------------------------------------------------------------------------------
QuadTree::~QuadTree()
{
	delete root_;
	delete bounds_;
}
//-------------------------------------------------------------------------------------------
QuadTree::QuadTree(const SDL_Rect& bounds, int maxDepth):
	bounds_(new SDL_Rect(bounds) ),
	root_( new TreeNode(bounds, this, 0, 0) ),
	maxDepth_( maxDepth),
	depth_ (0)
{
}
//-------------------------------------------------------------------------------------------
SDL_Rect*
QuadTree::bounds()
{
	return bounds_;
}
//-------------------------------------------------------------------------------------------
int
QuadTree::width() 
{
	return bounds_->w;
}
//-------------------------------------------------------------------------------------------
int
QuadTree::height()
{
	return bounds_->h;
}
//-------------------------------------------------------------------------------------------
void
QuadTree::insertBricks(std::vector<Brick*> objects)
{
	for (size_t i = 0; i < objects.size(); i++) {
		insertEntity(objects[i]);
	}
}
//-------------------------------------------------------------------------------------------
void
QuadTree::insertEntity(Entity * entity)
{
	root_->insertEntity(entity);
}
//-------------------------------------------------------------------------------------------
void
QuadTree::checkForDestroyedElements()
{
	root_->checkForDestroyedElements();
}
//-------------------------------------------------------------------------------------------
TreeNode*
QuadTree::locateEntityPositionInQuadTree(Entity* entity)
{
	return root_->locateEntityPositionInQuadTree(entity);
}
//-------------------------------------------------------------------------------------------
void
QuadTree::render(SDL_Renderer * sdlRenderer)
{
	if (root_) {
		root_->render(sdlRenderer);
	}
}
//-------------------------------------------------------------------------------------------
void
QuadTree::clear()
{
	root_->clear();
}
//-------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------
