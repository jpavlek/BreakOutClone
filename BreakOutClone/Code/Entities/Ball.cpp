#include "Ball.h"
#include "..\Graphics\GraphicsProperties.h"
#include <SDL.h>
#include <algorithm>
#include "..\Game\Game.h"

//-------------------------------------------------------------------------------------------
Ball::Ball():
	Entity("Ball") {
}
//-------------------------------------------------------------------------------------------
Ball::Ball(int x, int y, int diameter):
	Entity("Ball", x, y, diameter, diameter) {
}
//-------------------------------------------------------------------------------------------
Ball::Ball(int x, int y, int diameter, int velX, int velY, long textureId, SDL_Texture* sdlTexture):
	Entity("Ball", x, y, diameter, diameter, velX, velY, textureId, sdlTexture)
{
}
//-------------------------------------------------------------------------------------------
Ball::Ball(BallDescription* ballDesc):
	Entity("Ball", ballDesc->positionX_, ballDesc->positionY_, ballDesc->width_, ballDesc->height_, ballDesc->velocityX_, ballDesc->velocityY_, ballDesc->textureId_, ballDesc->texture_)
{
}
//-------------------------------------------------------------------------------------------
Ball::~Ball() {
}
//-------------------------------------------------------------------------------------------
void
Ball::setInitialConditions(int posX, int posY, int velX, int velY)
{
	initPosition(posX, posY, BALL_DIAMETER, BALL_DIAMETER);
	initVel(velX, velY);
}
//-------------------------------------------------------------------------------------------
void
Ball::move() 
{
	increasePosX(velX_);
	increasePosY(velY_);
}
//-------------------------------------------------------------------------------------------
// Check to see if a game object is going to hit the side of the screen //
bool
Ball::checkWallCollision(Entity* entity, Direction dir)
{
	int temp_x = 100; // stores the location of the entity after moving
	int temp_y = 100; // stores the location of the entity after moving

	// Get the location of the entity after it moves //
	switch (dir)
	{
		case LEFT:
		{
			temp_x = entity->posX() - entity->velX();
		} break;
		
		case RIGHT:
		{
			// Notice that we have to add the entities width to get its right(->) coordinate //
			temp_x = entity->posX() + entity->width() + entity->velX();
		} break;
		
		case UP: 
		{
			temp_y = entity->posY() - entity->velY();
		} break;
		
		case DOWN: 
		{
			// Notice that we have to add the entities width to get its right(->) coordinate //
			temp_y = entity->posY() + entity->height() + entity->velY();
		} break;
	}

	if ( (temp_x <= 0) || (temp_x >= SCREEN_WIDTH) ) {
		return true;
	}

	if ( (temp_y <= 0) || (temp_y >= SCREEN_HEIGHT) ) {
		return true;
	}

	return false;
}
//-------------------------------------------------------------------------------------------
void
Ball::handleWallCollisions()
{
	//handle wall collisions
	// If the ball is moving left, we see if it hits the wall. If does, //
	// we change its direction. We do the same thing if it's moving right. //
	if (((velX_ < 0) && checkWallCollision((Entity*) this, LEFT)) || ((velX_ > 0) && checkWallCollision((Entity*) this, RIGHT))) {
		velX_ = -velX_;
		playSoundEffect(1, owner_->resourceManager()->audioManager());
	} else if ( (velY_ < 0) && checkWallCollision((Entity*) this, UP) ) {
		velY_ = -velY_;
		playSoundEffect(1, owner_->resourceManager()->audioManager());
	} else if ( (velY_ > 0) && checkWallCollision((Entity*) this, DOWN) ) {
		owner_->onLifeLost();
		velY_ = -velY_;
		return;
	}
}
//-------------------------------------------------------------------------------------------
bool
Ball::checkPaddleCollision(Paddle* paddle) {

	if (velY() < 0) {
		return false;
	}

	if (posY() + height() < paddle->posY() ) {
		return false;
	}

	if ( posX()+width() < paddle->posX() ) {
		return false;
	}

	if ( posX()-width() > paddle->posX() + paddle->width() ) {
		return false;
	}

	// Check to see if ball is in Y range of the player's paddle. //
	// We check its speed to see if it's even moving towards the player's paddle. //
	if ( (velY() > 0) && (posY() + height() >= paddle->posY()) &&
		(posY() + height() <= paddle->posY() + paddle->height() ) )        // side hit
	{
		// If ball is in the X range of the paddle, return true. //
		if ( (posX() <= paddle->posX() + paddle->width() ) && (posX() + width() >= paddle->posX() ) )
		{
			return true;
		}
	}

	return false;
}
//-------------------------------------------------------------------------------------------
void
Ball::setDiameter(int diameter)
{ 
	diameter_ = diameter;
	width(diameter);
	height(diameter);
}
//-------------------------------------------------------------------------------------------
void
Ball::handlePaddleCollision(Paddle* paddle)
{
	//handle paddle collision
	if (checkPaddleCollision(paddle)) {
		// Get center location of paddle //
		int paddle_center = paddle->posX() + paddle->width() / 2;
		int ball_center = posX() + width() / 2;

		// Find the location on the paddle that the ball hit //
		int distanceXFromPaddleCenter = ball_center - paddle_center;

		// Increase X speed according to distance from center of paddle. //
		resolvePaddleCollision(distanceXFromPaddleCenter, paddle->width());
	}
}
//-------------------------------------------------------------------------------------------
void
Ball::resolvePaddleCollision(int distanceXFromPaddleCenter, int paddleWidth)
{
	//hit on the paddle corner
	if ( ( paddleWidth / 2 - abs(distanceXFromPaddleCenter) <= 2) && (distanceXFromPaddleCenter*velX() < 0) ) {
		velX(-velX());
	}

	//just bounce up
	velY(-velY());
	playSoundEffect(3, owner_->resourceManager()->audioManager());
	//modify direction and speed
	//velX(distanceXFromPaddleCenter / BALL_SPEED_MODIFIER);
	
}
//-------------------------------------------------------------------------------------------
bool
Ball::checkBrickWallCollision(BrickWall* brickWall)
{
	if (brickWall && intersectsRectangle(brickWall) ) {
		return true;
	}
	return false;
}
//-------------------------------------------------------------------------------------------
void
Ball::resolveBrickWallCollision(std::vector<Entity*> &ent)
{
	int maxContactPosY = 0;
	std::map<int, int> axisMap;
	for (size_t i = 0; i < ent.size(); i++) {
		Brick* brick = dynamic_cast<Brick*>(ent[i]);
		bool collidedWithBrick = checkBrickCollision(brick);
		if (!collidedWithBrick) {
			continue;
		}

		int axisAffected = resolveBrickCollision(brick);
		std::map<int, int>::const_iterator it = axisMap.find(axisAffected);
		if (it != axisMap.end()) {
			axisMap[axisAffected]++;
		} else {
			axisMap[axisAffected] = 1;
		}
		brick->onBrickHit();
		if (axisAffected == 2) {
			break;
		}
	}
	std::map<int, int>::const_iterator it = axisMap.find(0);
	if (it != axisMap.end()) {
		if (axisMap[0] % 2 == 0) {
			velX(-velX_);
		}
	}
	it = axisMap.find(1);
	if (it != axisMap.end()) {
		if (axisMap[1] % 2 == 0) {
			velY(-velY_);
		}
	}

}
//-------------------------------------------------------------------------------------------
bool
Ball::checkBrickCollision(Brick* brick)
{
	if (brick && intersectsRectangle(brick)) {
		return true;
	}
	return false;
}
//-------------------------------------------------------------------------------------------
int
Ball::resolveBrickCollision(Brick* brick)
{
	if (brick == nullptr) {
		return -1;
	}
	int xMin = std::max(posX(), brick->posX());
	int xMax = std::min(posX() + width(), brick->posX() + brick->width());
	int yMin = std::max(posY(), brick->posY());
	int yMax = std::min(posY() + height(), brick->posY() + brick->height());
	int collisionSegmentX = xMax - xMin;
	int collisionSegmentY = yMax - yMin;
	if (collisionSegmentX > collisionSegmentY) {
		velY(-velY());
		return 1;
	}
	else if (collisionSegmentX < collisionSegmentY) {
		velX(-velX());
		return 0;
	}
	else {
		//no collision, touching corners
		if (collisionSegmentX == 0) {
			return -1;
		}
		
		int relPosition = relativePosition(brick);
		if (relPosition == 0 || relPosition == 6) {
			if (velX() < 0) {
				velX(-velX_);
			}
		}

		if (relPosition == 2 || relPosition == 8) {
			if (velX() > 0) {
				velX(-velX_);
			}
		}
		
		if (relPosition == 0 || relPosition == 2) {
			if (velY() < 0) {
				velY(-velY_);
			}
		}

		if (relPosition == 6 || relPosition == 8) {
			if (velY() > 0) {
				velY(-velY_);
			}
		}

		return 2;
	}
}
//-------------------------------------------------------------------------------------------
void
Ball::update()
{
	SDL_Rect position = position_;
	move();
	handleWallCollisions();
	handlePaddleCollision( owner_->paddle() );
	if (checkBrickWallCollision( owner_->getBrickwall() ) ) {
		QuadTree* quadTree = owner_->getQuadTree();
		TreeNode* ballCollisionLocNode = quadTree->locateEntityPositionInQuadTree(this);
		owner_->setBallCollisionLocationNode(ballCollisionLocNode);
		if (ballCollisionLocNode == nullptr) {
			return;
		}
		std::vector<Entity*> ent = ballCollisionLocNode->retrieveEntities();
		std::sort(ent.begin(), ent.end());
		if (ent.size() > 0) {
			resolveBrickWallCollision(ent);
			PlayerProfile* playerProfile = owner_->playerProfile();
			playerProfile->updateScore(ent);
			owner_->updateHUD();
			owner_->checkForDestroyedBricks();
			bool levelFinished = owner_->checkLevelFinished();
			if (levelFinished) {
				owner_->setGameState(GameState::NEXT_LEVEL);
			}
		}
	}
	else {
		owner_->setBallCollisionLocationNode(0);
	}
	pos(position.x, position.y);
	move();

}
//-------------------------------------------------------------------------------------------