#pragma once

#include "Brick.h"
#include "..\Graphics\GraphicsProperties.h"
#include <SDL_mixer.h>

//-------------------------------------------------------------------------------------------
Brick::Brick():
	Entity(),
	id_(0),
	texturePathName_(""),
	hitSoundPathName_(""),
	breakSoundPathName_(""),
	hitPoints_(0),
	breakScore_(0),
	pointsLeft_(0)
{
	if (brickHitSoundEffect_ == 0) {
		int a = 0;
	}
}
//-------------------------------------------------------------------------------------------
Brick::Brick(int x, int y, int w, int h, char brickId, const char* txtureName, const char* hitSndName, const char* breakSndName, int hitPnts, int breakScr, long textureId, SDL_Texture* texture, Mix_Chunk* brickHitSnd, Mix_Chunk* brickBrokenSnd):
	Entity("Brick", x, y, w, h, 0, 0, textureId, texture),
	id_(brickId),
	texturePathName_(txtureName),
	hitSoundPathName_(hitSndName),
	breakSoundPathName_(breakSndName),
	hitPoints_(hitPnts),
	breakScore_(breakScr),
	pointsLeft_(hitPnts),
	brickHitSoundEffect_(brickHitSnd),
	brickBrokenSoundEffect_(brickBrokenSnd)
{
	if (brickHitSoundEffect_ == 0) {
		int a = 0;
	}
}
//-------------------------------------------------------------------------------------------
Brick::~Brick() {
}
//-------------------------------------------------------------------------------------------
void Brick::onBrickHit()
{
	playHitSound();
	if (id_ == 'I') {
		return;
	}

	decreasePointsLeft();
}
//-------------------------------------------------------------------------------------------
void
Brick::decreasePointsLeft() {
	pointsLeft_--;
}
//-------------------------------------------------------------------------------------------
void
Brick::playHitSound()
{
	if (brickHitSoundEffect_ == 0) {
		printf("No brick hit sound effect to play.\n");
		return;
	}
	Mix_PlayChannel(-1, brickHitSoundEffect_, 0);
}
//-------------------------------------------------------------------------------------------
void
Brick::playBreakSound()
{
	Mix_PlayChannel(-1, brickBrokenSoundEffect_, 0);
}
//-------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------