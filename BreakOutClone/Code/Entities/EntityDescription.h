#pragma once

#include <SDL.h>

class EntityDescription
{
public:
	EntityDescription();
	virtual ~EntityDescription();
	virtual void setInitialConditions(int x, int y, int w, int h, int velX, int velY);
	void setPosition(int x, int y, int w, int h);
	void setVelocity(int velX, int velY);
	void setWidth(int width);
	void setHeight(int height);
	void setTexture(SDL_Texture* texture);
	void setTextureId(long textureId);
	int positionX_{ -1 };
	int positionY_{ -1 };
	int width_{ -1 };
	int height_{ -1 };
	int velocityX_{ 0 };
	int velocityY_{ 0 };
	SDL_Texture* texture_{ NULL };
	long textureId_{ 0 };
};