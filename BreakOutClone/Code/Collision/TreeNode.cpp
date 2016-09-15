#include "TreeNode.h"
#include "QuadTree.h"

//-------------------------------------------------------------------------------------------
TreeNode::TreeNode()
:	quadTree_(0), 
	bounds_( {0, 0, 0, 0} ),
	depth_(0),
	parentNode_(0),
	isLeaf_(true)
{
	boundsNormalized_ = new DoubleRect(0.0, 0.0, 0.0, 0.0);
}
//-------------------------------------------------------------------------------------------
TreeNode::TreeNode(const SDL_Rect &bounds, QuadTree* owner, int depth, TreeNode* parentNode)
:	quadTree_(owner),
	bounds_( {bounds.x, bounds.y, bounds.w, bounds.h} ),
	depth_(depth),
	parentNode_(parentNode),
	isLeaf_(true)
{
	SDL_Rect* quadTreeBounds = quadTree_->bounds();
	int width = quadTreeBounds->w;
	int height = quadTreeBounds->h;
	boundsNormalized_ = new DoubleRect(1.0*bounds.x/width, 1.0*bounds.y/height, 1.0*bounds.w/width, 1.0*bounds.h/height);

	for (size_t i = 0; i < QUAD; i++) {
		subNodes_.push_back(0);
	}

	int a = 0;
}
//-------------------------------------------------------------------------------------------
TreeNode::~TreeNode()
{
	for (size_t i = 0; i < subNodes_.size(); i++) {
		delete subNodes_[i];
		subNodes_[i] = 0;
	}
	delete boundsNormalized_;
}
//-------------------------------------------------------------------------------------------
bool 
TreeNode::contains(TreeNode* subQuad, Entity* object)
{
	if (subQuad == 0) {
		return false;
	}

	return !(object->posX() < subQuad->bounds_.x ||
		object->posY() < subQuad->bounds().y ||
		object->posX() > subQuad->bounds().x + subQuad->bounds().w ||
		object->posY() > subQuad->bounds().y + subQuad->bounds().h ||
		object->posX() + object->width()  < subQuad->bounds().x ||
		object->posY() + object->height() < subQuad->bounds().y ||
		object->posX() + object->width()  > subQuad->bounds().x + subQuad->bounds().w ||
		object->posY() + object->height() > subQuad->bounds().y + subQuad->bounds().h);
}
//-------------------------------------------------------------------------------------------
void TreeNode::split()
{
	const int halfWidth = (int) (round (bounds().w / 2));
	const int halfHeight = (int) (round( bounds().h / 2));
	SDL_Rect NW = { bounds().x, bounds().y, halfWidth, halfHeight };
	SDL_Rect NE = { bounds().x + halfWidth, bounds().y, halfWidth, halfHeight };
	SDL_Rect SW = { bounds().x, bounds().y + halfHeight, halfWidth, halfHeight };
	SDL_Rect SE = { bounds().x + halfWidth, bounds().y + halfHeight, halfWidth, halfHeight };
	subNodes_[0] = new TreeNode(NW, quadTree_, depth_ + 1, this);
	subNodes_[1] = new TreeNode(NE, quadTree_, depth_ + 1, this);
	subNodes_[2] = new TreeNode(SW, quadTree_, depth_ + 1, this);
	subNodes_[3] = new TreeNode(SW, quadTree_, depth_ + 1, this);
}
//-------------------------------------------------------------------------------------------
void
TreeNode::insertEntity(Entity* entity)
{
	if (depth_ == quadTree_->maxDepth() ) {
		objects_.push_back(entity);
		return;
	}

	if (subNodes_[0] == 0 || subNodes_[1] == 0 || subNodes_[2] == 0 || subNodes_[3] == 0) {
		split();
	}

	if (contains(subNodes_[0], entity)) {
		subNodes_[0]->insertEntity(entity);
		return;
	} else if (contains(subNodes_[1], entity)) {
		subNodes_[1]->insertEntity(entity);
		return;
	} else if (contains(subNodes_[2], entity)) {
		subNodes_[2]->insertEntity(entity);
		return;
	} else if (contains(subNodes_[3], entity)) {
		subNodes_[3]->insertEntity(entity);
		return;
	}
	
	if (contains(this, entity)) {
		objects_.push_back(entity);
	}
}
//-------------------------------------------------------------------------------------------
TreeNode*
TreeNode::locateEntityPositionInQuadTree(Entity* entity)
{
	if (subNodes_[0] && contains(subNodes_[0], entity)) {
		return subNodes_[0]->locateEntityPositionInQuadTree(entity);
	}
	else if (subNodes_[1] && contains(subNodes_[1], entity)) {
		return subNodes_[1]->locateEntityPositionInQuadTree(entity);
	}
	else if (subNodes_[2] && contains(subNodes_[2], entity)) {
		return subNodes_[2]->locateEntityPositionInQuadTree(entity);
	}
	else if (subNodes_[3] && contains(subNodes_[3], entity)) {
		return subNodes_[3]->locateEntityPositionInQuadTree(entity);
	}
	else if (contains(this, entity)) {
		return this;
	}

	return 0;
}
//-------------------------------------------------------------------------------------------
void
TreeNode::checkForDestroyedElements()
{
	if (objects_.size() > 0) {
		for (int i = objects_.size()-1; i >=0; i--) {
			Brick* brick = dynamic_cast<Brick*>(objects_[i]);
			if (brick->pointsLeft() <= 0) {
				objects_.erase(objects_.begin() + i);
			}
		}
	}

	if (subNodes_.size()) {

		if (subNodes_[0]) {
			subNodes_[0]->checkForDestroyedElements();
		}
		if (subNodes_[1]) {
			subNodes_[1]->checkForDestroyedElements();
		}
		if (subNodes_[2]) {
			subNodes_[2]->checkForDestroyedElements();
		}
		if (subNodes_[3]) {
			subNodes_[3]->checkForDestroyedElements();
		}
	}
}
//-------------------------------------------------------------------------------------------
void
TreeNode::render(SDL_Renderer* sdlRenderer)
{
	//Render green outlined quad
	if (objects_.size() > 0) {
		SDL_SetRenderDrawColor(sdlRenderer, 0x00, 0xFF, 0x00, 0xFF);
		SDL_RenderDrawRect(sdlRenderer, &bounds_);
	} else if (objects_.size() == 0) {
		SDL_SetRenderDrawColor(sdlRenderer, 0x7F, 0x7F, 0x7F, 0xFF);
		SDL_RenderDrawRect(sdlRenderer, &bounds_);
	}
	for (int i = 0; i < 4; i++) {
		if (subNodes_[i]) {
			subNodes_[i]->render(sdlRenderer);
		}
	}
	SDL_SetRenderDrawColor(sdlRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
}
//-------------------------------------------------------------------------------------------
void
TreeNode::debugRender(SDL_Renderer* sdlRenderer, int r, int g, int b, int a)
{
	SDL_SetRenderDrawColor(sdlRenderer, r, g, b, a);
	SDL_RenderDrawRect(sdlRenderer, &bounds_);
	SDL_SetRenderDrawColor(sdlRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
}
//-------------------------------------------------------------------------------------------
std::vector<Entity*>
TreeNode::retrieveEntities()
{
	std::vector<Entity*> entities;
	if (objects_.size()) {
		for (size_t i = 0; i < objects_.size(); i++) {
			entities.push_back(objects_[i]);
		}
	}
	std::vector<Entity*> subEntities;
	if (subNodes_.size()) {
	
		if (subNodes_[0]) {
			std::vector<Entity*> subEntities0 = subNodes_[0]->retrieveEntities();
			subEntities.insert(subEntities.end(), subEntities0.begin(), subEntities0.end());
		}
		if (subNodes_[1]) {
			std::vector<Entity*> subEntities1 = subNodes_[1]->retrieveEntities();
			subEntities.insert(subEntities.end(), subEntities1.begin(), subEntities1.end());
		}
		if (subNodes_[2]) {
			std::vector<Entity*> subEntities2 = subNodes_[2]->retrieveEntities();
			subEntities.insert(subEntities.end(), subEntities2.begin(), subEntities2.end());
		}
		if (subNodes_[3]) {
			std::vector<Entity*> subEntities3 = subNodes_[3]->retrieveEntities();
			subEntities.insert(subEntities.end(), subEntities3.begin(), subEntities3.end());
		}
	}
	entities.insert(entities.end(), subEntities.begin(), subEntities.end());
	return entities;
}
//-------------------------------------------------------------------------------------------
void
TreeNode::clear()
{
	if (depth_ == quadTree_->maxDepth()) {
		objects_.clear();
		return;
	} else {
		if (subNodes_[0]) {
			subNodes_[0]->clear();
		}
		if (subNodes_[1]) {
			subNodes_[1]->clear();
		}
		if (subNodes_[2]) {
			subNodes_[2]->clear();
		}
		if (subNodes_[3]) {
			subNodes_[3]->clear();
		}
	}
	
	if (!objects_.empty()) {
		objects_.clear();
	}
}
//-------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------