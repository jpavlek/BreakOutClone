#pragma once

#include "Entity.h"
#include "Direction.h"
#include "PaddleDescription.h"

//-------------------------------------------------------------------------------------------
class Paddle: public Entity {
public:
	Paddle();
	Paddle(int x, int y, int w, int h);
	Paddle(PaddleDescription* paddleDesc);
	void move();
	void increasePosX(int deltaX) { position_.x += deltaX; }
	void increasePosY(int deltaY) { position_.y += deltaY; }
	bool checkWallCollision(Entity* entity, Direction dir);
	void increaseVelX();
	void decreaseVelX();
	void setMaxVelocityLeft();
	void setMaxVelocityRight();
	void limitPosX();
	void allignWithMousePosition(int x);
	virtual ~Paddle();
};
//-------------------------------------------------------------------------------------------