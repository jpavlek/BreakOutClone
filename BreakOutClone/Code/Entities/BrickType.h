#pragma once
#include <SDL.h>
#include "..\ResourceManager\AudioDescription.h"

struct BrickType
{
	char id_;
	const char* texturePathName_;
	const char* hitSoundPathName_;
	const char* breakSoundPathName_;
	int hitPoints_;
	int breakScore_;
	BrickType(char id, const char* textureName, int hitPts, const char* hitSoundName, const char* breakSoundName, int breakScr, SDL_Texture* texture, long textureID, AudioDescription hitSoundDesc, AudioDescription breakSoundDesc);
	BrickType();
	void setTexture(SDL_Texture* texture);
	void setTextureId(long textureId);
	SDL_Texture* texture_;
	long textureId_;
	AudioDescription hitSound;
	AudioDescription breakSound;
};