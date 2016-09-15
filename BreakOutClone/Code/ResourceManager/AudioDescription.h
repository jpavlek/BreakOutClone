#pragma once
#include <SDL_mixer.h>

struct AudioDescription
{
	AudioDescription();
	AudioDescription(Mix_Chunk* sfx, long sfxId);
	Mix_Chunk* soundEffect_;
	long soundEffectId_;
};