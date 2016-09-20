#pragma once
#include "..\Graphics\TextureManager.h"
#include <map>
#include <SDL_ttf.h>
#include "AudioManager.h"

class ResourceManager {
public:
	static long textureCount_;
	static long fontsCount_;
	static long sfxCount_;

	ResourceManager();
	~ResourceManager();
	
	//getters
	TextureManager* textureManager() { return textureManager_; }
	AudioManager* audioManager() { return audioManager_; }
	SDL_Texture* getTexture(long textureId);
	TTF_Font* getFont(long fontId);
	Mix_Chunk* getSoundEffect(long sfxId);

	long addTexture(std::string fullPathTextureFileName, SDL_Renderer* pRenderer);
	long addFont(std::string fullPathFontFileName, SDL_Renderer* pRenderer);
	long addTextureFromFontRenderedText(std::string fullPathTextureFileName, long fontId, SDL_Color textColor, SDL_Renderer* pRenderer);
	long addSoundEffect(std::string fullPathSFXFileName);
	SDL_Texture* updateTextureFromFontRenderedText(std::string textureText, long fontId, long textureId, SDL_Color textColor, SDL_Renderer* pRenderer);

private:
	TextureManager* textureManager_;
	AudioManager* audioManager_;
};
