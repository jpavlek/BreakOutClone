#include "BrickType.h"
#include <iostream>
//-------------------------------------------------------------------------------------------
BrickType::BrickType(char brickId, const char* textureName, int hitPts, const char* hitSoundName, const char* breakSoundName, int breakScr, SDL_Texture* texture, long textureID, AudioDescription hitSoundDesc, AudioDescription breakSoundDesc)
	:id_(brickId),
	texturePathName_(textureName),
	hitPoints_(hitPts),
	hitSoundPathName_(hitSoundName),
	breakSoundPathName_(breakSoundName),
	breakScore_(breakScr),
	texture_(texture),
	textureId_(textureID),
	hitSound(hitSoundDesc),
	breakSound(breakSoundDesc)
{
	if (hitSound.soundEffect_ == 0) {
		printf("No hit sound effect for BrickType id=\'%c\'.\n", id_);
	}
	if (breakSound.soundEffect_ == 0) {
		printf("No break sound effect for BrickType id=\'%c\'.\n", id_);
	}
}
//-------------------------------------------------------------------------------------------
BrickType::BrickType() 
:	id_('?'),
	texturePathName_(""),
	hitPoints_(0),
	hitSoundPathName_(""),
	breakSoundPathName_(""),
	breakScore_(0),
	texture_(0),
	textureId_(0)
{
}
//-------------------------------------------------------------------------------------------
void
BrickType::setTexture(SDL_Texture* texture_)
{

}
void
BrickType::setTextureId(long textureId_)
{

}
//-------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------