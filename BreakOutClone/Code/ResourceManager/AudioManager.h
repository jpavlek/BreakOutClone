#pragma once

#pragma once

#include <map>
#include <string>
#include <SDL_mixer.h>

class AudioManager
{
public:
	static AudioManager* Instance() {
		if (s_pInstance == 0) {
			s_pInstance = new AudioManager();
			return s_pInstance;
		}
		return s_pInstance;
	}

	static void ResetInstance() {
		delete s_pInstance;
		s_pInstance = NULL;
	}

	bool loadSoundEffect(std::string fullPathFileName, long id);
	Mix_Chunk* getSoundEffect(long soundEffectId);
	std::map<long, Mix_Chunk*>* soundEffectsMap() { return &soundEffectsMap_; }
	~AudioManager();
private:
	std::map<long, Mix_Chunk*> soundEffectsMap_;
	AudioManager() {}
	static AudioManager* AudioManager::s_pInstance;
};
