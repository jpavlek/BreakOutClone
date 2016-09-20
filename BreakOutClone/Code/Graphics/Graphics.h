#pragma once

#include <vector>
#include "TextureManager.h"

//Abstract base class for Graphics
class Graphics {
public:
	Graphics();
	Graphics(char* graphicsName);
	virtual bool initGraphics() = 0;
	virtual bool closeGraphics() = 0;
	virtual bool unloadMedia(TextureManager* textureManager) = 0;
	virtual void setBackgroundColor(int r, int g, int b, int alpha) = 0;
private:
	char* graphicsName_{ "Graphics" };
};
