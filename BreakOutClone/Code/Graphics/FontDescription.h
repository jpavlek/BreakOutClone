#pragma once
#include <SDL.h>
#include <SDL_ttf.h>

struct FontDescription
{
	FontDescription(TTF_Font* font, long fontId);
	TTF_Font* font_;
	long fontId_;
};