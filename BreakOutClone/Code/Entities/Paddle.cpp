#pragma once

#include "Paddle.h"
#include "../Graphics/GraphicsProperties.h"

//-------------------------------------------------------------------------------------------
Paddle::Paddle(): Entity()
{
	entityName_ = "Paddle";
}
//-------------------------------------------------------------------------------------------
Paddle::Paddle(int x, int y, int w, int h)
	: Entity(x, y, w, h)
{
	entityName_ = "Paddle";
}
Paddle::Paddle(PaddleDescription* paddleDesc)
	: Entity("Paddle", paddleDesc->positionX_, paddleDesc->positionY_, paddleDesc->width_, paddleDesc->height_, paddleDesc->velocityX_, paddleDesc->velocityY_, paddleDesc->textureId_, paddleDesc->texture_)
{
}
//-------------------------------------------------------------------------------------------
Paddle::~Paddle() {
}
//-------------------------------------------------------------------------------------------
void
Paddle::setMaxVelocityLeft()
{
	velX(-MAX_VEL_X);
}
void
Paddle::setMaxVelocityRight()
{
	velX(MAX_VEL_X);
}
void
Paddle::move() 
{
	// If the ball is moving left, we see if it hits the wall. If does, //
	// we change its direction. We do the same thing if it's moving right. //
	/*
	if ( ( (velX_ < 0) && checkWallCollision( (Entity*) this, LEFT) ) || ( (velX_ > 0) && checkWallCollision( (Entity*) this, RIGHT) ) )
	{
		velX_ = -velX_;
		velX_ = 0;
	}
	*/
	increasePosX(velX_);
	velX_ = 0;
}
//-------------------------------------------------------------------------------------------
void
Paddle::allignWithMousePosition(int x) {
	int paddle_posX = x - PADDLE_WIDTH / 2;
	posX(paddle_posX);
	limitPosX();
}
//-------------------------------------------------------------------------------------------
void
Paddle::limitPosX() 
{
	if (posX() < 0) {
		posX(0);
	}
	if (posX() > SCREEN_WIDTH - PADDLE_WIDTH) {
		posX(SCREEN_WIDTH - PADDLE_WIDTH);
	}
}
//-------------------------------------------------------------------------------------------
// Check to see if a game object is going to hit the side of the screen //
bool
Paddle::checkWallCollision(Entity* entity, Direction dir)
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

	if ( (temp_x <= 0) || (temp_x >= SCREEN_WIDTH) )
	{
		return true;
	}

	if ( (temp_y <= 0) || (temp_y >= SCREEN_HEIGHT) )
	{
		return true;
	}

	return false;
}
//-------------------------------------------------------------------------------------------
void
Paddle::increaseVelX()
{	
	velX_++; 
	if (velX_ > MAX_VEL_X) {
		velX_ = MAX_VEL_X;
	}
}
//-------------------------------------------------------------------------------------------
void
Paddle::decreaseVelX()
{ 
	velX_--;
	if (velX_ < -MAX_VEL_X) {
		velX_ = -MAX_VEL_X;
	}
}
//-------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------