#pragma once

#include "Ball.h"
#include "..\Graphics\GraphicsProperties.h"
#include <SDL.h>
#include <algorithm>
#include "..\Game\Game.h"

//-------------------------------------------------------------------------------------------
Ball::Ball():
	Entity() {
}
//-------------------------------------------------------------------------------------------
Ball::Ball(int x, int y, int diameter):
	Entity(x, y, diameter, diameter) {
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
Ball::move(/*Entity* paddle*/) 
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
	}

	if ( (velY_ < 0) && checkWallCollision((Entity*) this, UP) ) {
		velY_ = -velY_;
	}

	if ( (velY_ > 0) && checkWallCollision((Entity*) this, DOWN) ) {
		owner_->onLifeLost();
		velY_ = -velY_;
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
		resolvePaddleCollision(distanceXFromPaddleCenter);
	}
}
//-------------------------------------------------------------------------------------------
void
Ball::resolvePaddleCollision(int distanceXFromPaddleCenter)
{
	velX(distanceXFromPaddleCenter / BALL_SPEED_MODIFIER);
	velY(-velY());
}
//-------------------------------------------------------------------------------------------
bool
Ball::checkBrickWallCollision(BrickWall* brickWall)
{
	if ( !intersectsRectangle(brickWall) ) {
		return false;
	}

	return true;
}
//-------------------------------------------------------------------------------------------
void
Ball::resolveBrickWallCollision(std::vector<Entity*> &ent)
{
	for (size_t i = 0; i < ent.size(); i++) {
		Brick* brick = dynamic_cast<Brick*>(ent[i]);
		bool collidedWithBrick = checkBrickCollision(brick);
		if (!collidedWithBrick) {
			continue;
		}
		resolveBrickCollision(brick);

		brick->onBrickHit();
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
void
Ball::resolveBrickCollision(Brick* brick)
{
	if (brick == 0) {
		return;
	}
	int xMin = std::max(posX(), brick->posX());
	int xMax = std::min(posX() + width(), brick->posX() + brick->width());
	int yMin = std::max(posY(), brick->posY());
	int yMax = std::min(posY() + height(), brick->posY() + brick->height());
	int collisionSegmentX = xMax - xMin;
	int collisionSegmentY = yMax - yMin;
	if (collisionSegmentX > collisionSegmentY) {
		velY(-velY());
	}
	else if (collisionSegmentX < collisionSegmentY) {
		velX(-velX());
	}
	else {
		velY(-velY());
		velX(-velX());
	}
}
//-------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------