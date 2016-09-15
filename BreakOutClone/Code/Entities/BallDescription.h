#pragma once
#include "EntityDescription.h"

const int BALL_DIAMETER = 20;
const int BALL_SPEED_MODIFIER = 8;  // divide location on paddle by this
const int BALL_SPEED = 1;

class BallDescription : public EntityDescription
{
public:
	BallDescription();
	~BallDescription();
	void setDiameter(int diameter);
	void setInitialConditions(int positionX, int positionY, int velocityX, int velocityY);
	void setDefaultValues();
	int diameter_;
};