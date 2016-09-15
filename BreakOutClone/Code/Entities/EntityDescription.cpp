#include "EntityDescription.h"
//-------------------------------------------------------------------------------------------
void
EntityDescription::setInitialConditions(int x, int y, int w, int h, int velX, int velY)
{
	setPosition(x, y, w, h);
	setVelocity(velX, velY);
}
//-------------------------------------------------------------------------------------------
EntityDescription::EntityDescription()
	:	positionX_(-1),
		positionY_(-1),
		width_(-1),
		height_(-1),
		velocityX_(-1),
		velocityY_(-1),
		texture_(NULL)
{
}
EntityDescription::~EntityDescription()
{
}
//-------------------------------------------------------------------------------------------
void 
EntityDescription::setPosition(int posX, int posY, int width, int height)
{
	positionX_ = posX;
	positionY_ = posY;
	width_ = width;
	height_ = height;
}
//-------------------------------------------------------------------------------------------
void
EntityDescription::setVelocity(int velX, int velY)
{
	velocityX_ = velX;
	velocityY_ = velY;
}
//-------------------------------------------------------------------------------------------
void
EntityDescription::setWidth(int width)
{
	width_ = width;
}
//-------------------------------------------------------------------------------------------
void
EntityDescription::setHeight(int height)
{
	height_ = height;
}
//-------------------------------------------------------------------------------------------
void
EntityDescription::setTexture(SDL_Texture* texture)
{
	texture_ = texture;
}
//-------------------------------------------------------------------------------------------
void
EntityDescription::setTextureId(long textureId)
{
	textureId_ = textureId;
}
//-------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------