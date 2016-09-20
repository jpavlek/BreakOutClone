#include "ResourceManager.h"
#include <map>
#include <string>

long ResourceManager::textureCount_ = 0;
long ResourceManager::fontsCount_ = 0;
long ResourceManager::sfxCount_ = 0;
//-------------------------------------------------------------------------------------------
ResourceManager::ResourceManager():
	textureManager_( TextureManager::Instance() ),
	audioManager_( AudioManager::Instance() )
{	
}
//-------------------------------------------------------------------------------------------
ResourceManager::~ResourceManager()
{
	TextureManager::ResetInstance();
	AudioManager::ResetInstance();
}
//-------------------------------------------------------------------------------------------
SDL_Texture*
ResourceManager::getTexture(long textureId)
{
	if (textureManager_ == nullptr) {
		return nullptr;
	}

	return textureManager_->getTexture(textureId);	
}
//-------------------------------------------------------------------------------------------
TTF_Font*
ResourceManager::getFont(long fontId)
{
	if (textureManager_ == nullptr) {
		return nullptr;
	}

	return textureManager_->getFont(fontId);
}
//-------------------------------------------------------------------------------------------
Mix_Chunk*
ResourceManager::getSoundEffect(long sfxId)
{
	if (audioManager_ == nullptr) {
		return nullptr;
	}

	return audioManager_->getSoundEffect(sfxId);
}
//-------------------------------------------------------------------------------------------
long 
ResourceManager::addTexture(std::string fullPathTextureFileName, SDL_Renderer* pRenderer)
{
	textureCount_++;
	bool textureLoaded = textureManager_->loadTexture(fullPathTextureFileName, textureCount_, pRenderer);
	return textureCount_;
}
//-------------------------------------------------------------------------------------------
long
ResourceManager::addTextureFromFontRenderedText(std::string textureText, long fontId, SDL_Color textColor, SDL_Renderer* pRenderer)
{
	textureCount_++;
	bool textureLoaded = textureManager_->loadTextureFromRenderedText(textureText, fontId, textureCount_, textColor, pRenderer);
	return textureCount_;
}
//-------------------------------------------------------------------------------------------
SDL_Texture*
ResourceManager::updateTextureFromFontRenderedText(std::string textureText, long fontId, long textureId, SDL_Color textColor, SDL_Renderer* pRenderer)
{
	bool textureLoaded = textureManager_->loadTextureFromRenderedText(textureText, fontId, textureId, textColor, pRenderer);
	SDL_Texture* texture = getTexture(textureId);
	return texture;
}
//-------------------------------------------------------------------------------------------
long
ResourceManager::addFont(std::string fullPathFontFileName, SDL_Renderer* pRenderer)
{
	fontsCount_++;
	bool fontLoaded = textureManager_->loadFont(fullPathFontFileName, fontsCount_, pRenderer);
	return fontsCount_;
}
//-------------------------------------------------------------------------------------------
long
ResourceManager::addSoundEffect(std::string fullPathSFXFileName)
{
	sfxCount_++;
	bool sfxLoaded = audioManager_->loadSoundEffect(fullPathSFXFileName, sfxCount_);
	if (!sfxLoaded) {
		printf("Failed to add sound effect \'%s\' to resource manager! SDL_mixer Error: %s\n", fullPathSFXFileName.c_str(), Mix_GetError());
	}
	return sfxCount_;
}
//-------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------