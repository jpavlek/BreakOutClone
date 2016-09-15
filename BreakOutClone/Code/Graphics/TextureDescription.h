#pragma once
#include <SDL.h>

struct TextureDescription
{
	TextureDescription(SDL_Texture* texture, long textureId);
	SDL_Texture* texture_;
	long textureId_;
};