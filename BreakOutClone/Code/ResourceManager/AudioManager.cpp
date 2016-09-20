#include "AudioManager.h"

AudioManager* AudioManager::s_pInstance = 0;
//-------------------------------------------------------------------------------------------
AudioManager::~AudioManager()
{
	for (auto it = soundEffectsMap_.begin(); it != soundEffectsMap_.end(); ) {
		Mix_FreeChunk(it->second);
		soundEffectsMap_.erase(it++);
	}
}
//-------------------------------------------------------------------------------------------
Mix_Chunk*
AudioManager::getSoundEffect(long soundEffectId)
{
	std::map<long, Mix_Chunk*>::const_iterator it = soundEffectsMap_.find(soundEffectId);
	if (it != soundEffectsMap_.end()) {
		return soundEffectsMap_[soundEffectId];
	}
	return 0;

}
//-------------------------------------------------------------------------------------------
bool
AudioManager::loadSoundEffect(std::string fullPathFileName, long id)
{
	Mix_Chunk* sndEffX = Mix_LoadWAV( fullPathFileName.c_str() );

	if (sndEffX != 0) {
		soundEffectsMap_[id] = sndEffX;
		return true;
	}

	printf("Failed to load sound effect! SDL_mixer Error: %s\n", Mix_GetError());
	return false;
}
//-------------------------------------------------------------------------------------------
void
AudioManager::playSoundEffect(long soundEffectId)
{
	Mix_Chunk* sfx = getSoundEffect(soundEffectId);
	if (sfx == nullptr) {
		return;
	}
	Mix_PlayChannel(-1, sfx, 0);
}
//-------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------