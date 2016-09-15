#pragma once

#include "Entity.h"
#include <SDL_mixer.h>

//-------------------------------------------------------------------------------------------
class Brick: public Entity {
public:
	void decreasePointsLeft();
	Brick();
	Brick(int x, int y, int w, int h, char brickId, const char* txtureName, const char* hitSndName, const char* breakSndName, int hitPnts, int breakScr, long brickTypeId, SDL_Texture* texture, Mix_Chunk* brickHitSnd, Mix_Chunk* brickBrokenSnd);
	virtual ~Brick();
	int pointsLeft() { return pointsLeft_; }
	char id() { return id_; }
	int breakScore() { return breakScore_; }
	void onBrickHit();
	void playHitSound();
	void playBreakSound();
private:
	char id_;
	const char* texturePathName_{ "" };
	const char* hitSoundPathName_{ "" };
	const char* breakSoundPathName_{ "" };
	int hitPoints_{ 0 };
	int breakScore_{ 0 };
	int pointsLeft_{ 0 };
	Mix_Chunk* brickHitSoundEffect_{ 0 };
	Mix_Chunk* brickBrokenSoundEffect_{ 0 };
};