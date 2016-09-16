#include "AudioDescription.h"
//-------------------------------------------------------------------------------------------
AudioDescription::AudioDescription(Mix_Chunk* sfx, long sfxId)
	:soundEffect_(sfx),
	soundEffectId_(sfxId)
{
}
//-------------------------------------------------------------------------------------------
AudioDescription::AudioDescription():
	soundEffect_(0),
	soundEffectId_(0)
{
}
//-------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------