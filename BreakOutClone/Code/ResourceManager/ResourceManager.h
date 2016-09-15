#pragma once
#include <map>
#include "..\Graphics\TextureManager.h"
#include <SDL_ttf.h>
#include "AudioManager.h"

class ResourceManager {
public:
	ResourceManager();
	~ResourceManager();
	static long textureCount_;
	static long fontsCount_;
	static long sfxCount_;
	TextureManager* textureManager() { return textureManager_; }
	AudioManager* audioManager() { return audioManager_; }
	long addTexture(std::string fullPathTextureFileName, SDL_Renderer* pRenderer);
	long addFont(std::string fullPathFontFileName, SDL_Renderer* pRenderer);
	long addTextureFromFontRenderedText(std::string fullPathTextureFileName, long fontId, SDL_Color textColor, SDL_Renderer* pRenderer);
	SDL_Texture* updateTextureFromFontRenderedText(std::string textureText, long fontId, long textureId, SDL_Color textColor, SDL_Renderer* pRenderer);
	SDL_Texture* getTexture(long textureId);
	TTF_Font* getFont(long fontId);
	Mix_Chunk* getSoundEffect(long sfxId);
	long addSoundEffect(std::string fullPathSFXFileName);
private:
	TextureManager* textureManager_;
	AudioManager* audioManager_;
};
