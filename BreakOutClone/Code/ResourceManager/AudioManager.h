#pragma once

#include <map>
#include <string>
#include <SDL_mixer.h>

class AudioManager
{
public:
	static AudioManager* Instance() {
		if (s_pInstance == nullptr) {
			s_pInstance = new AudioManager();
			return s_pInstance;
		}
		return s_pInstance;
	}

	static void ResetInstance() {
		delete s_pInstance;
		s_pInstance = nullptr;
	}

	~AudioManager();
	Mix_Chunk* getSoundEffect(long soundEffectId);
	std::map<long, Mix_Chunk*>* soundEffectsMap() { return &soundEffectsMap_; }
	bool loadSoundEffect(std::string fullPathFileName, long id);
	void playSoundEffect(long soundEffectId);
private:
	static AudioManager* AudioManager::s_pInstance;
	AudioManager() {}
	std::map<long, Mix_Chunk*> soundEffectsMap_;
};
