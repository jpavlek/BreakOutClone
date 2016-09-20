#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <map>
#include <string>

class TextureManager
{
public:
	static TextureManager* Instance() {
		if (s_pInstance == nullptr) {
			s_pInstance = new TextureManager();
			return s_pInstance; 
		}
		return s_pInstance;
	}

	static void ResetInstance() {
		delete s_pInstance;
		s_pInstance = nullptr;
	}

	bool loadTexture(std::string fullPathFileName, long id, SDL_Renderer* pRenderer);
	bool loadFont(std::string fullPathFontFileName, long id, SDL_Renderer* pRenderer);
	bool loadTextureFromRenderedText(std::string textureText, long fontId, long textureId, SDL_Color textColor, SDL_Renderer* pRenderer);
	SDL_Texture* getTexture(long textureId);
	TTF_Font* getFont(long fontId);
	std::map<long, SDL_Texture*>* textureMap() { return &textureMap_; }
	void render(long id, int x, int y, int width, int height, SDL_Renderer *pRenderer, SDL_RendererFlip flip = SDL_FLIP_NONE);
	~TextureManager();
private:
	std::map<long, SDL_Texture*> textureMap_;
	TextureManager() {}
	static TextureManager* TextureManager::s_pInstance;
	std::map<long, TTF_Font*> fontsMap_;
};
