#include "BallDescription.h"
#include "..\Graphics\GraphicsProperties.h"

//-------------------------------------------------------------------------------------------
BallDescription::BallDescription():
	EntityDescription()
{
}
BallDescription::~BallDescription()
{
}
//-------------------------------------------------------------------------------------------
void
BallDescription::setDiameter(int diameter)
{
	diameter_ = diameter;
	width_ = diameter;
	height_ = diameter;
}
//-------------------------------------------------------------------------------------------
void
BallDescription::setInitialConditions(int positionX, int positionY, int velocityX, int velocityY)
{
	positionX_ = positionX;
	positionY_ = positionY;
	velocityX_ = velocityX;
	velocityY_ = velocityY;
}
//-------------------------------------------------------------------------------------------
void
BallDescription::setDefaultValues()
{
	setInitialConditions((SCREEN_WIDTH - BALL_DIAMETER) / 2, (SCREEN_HEIGHT - BALL_DIAMETER) / 2, BALL_SPEED, BALL_SPEED);
	setDiameter(BALL_DIAMETER);
}
//-------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------
