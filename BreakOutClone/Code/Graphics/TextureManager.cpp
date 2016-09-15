#include "TextureManager.h"
#include <SDL_image.h>

TextureManager* TextureManager::s_pInstance = 0;
//-------------------------------------------------------------------------------------------
bool
TextureManager::loadTexture(std::string fullPathFileName, long id, SDL_Renderer* pRenderer)
{
	SDL_Surface* pTempSurface = IMG_Load(fullPathFileName.c_str());
	if (pTempSurface == 0) {
		return false; 
	}
	SDL_Texture* pTexture = SDL_CreateTextureFromSurface(pRenderer, pTempSurface);
	SDL_FreeSurface(pTempSurface);

	if (pTexture != 0)  {
		textureMap_[id] = pTexture;
		return true;  
	}
	return false; 
}
//-------------------------------------------------------------------------------------------
bool
TextureManager::loadFont(std::string fullPathFontFileName, long id, SDL_Renderer* pRenderer)
{
	TTF_Font* font = TTF_OpenFont(fullPathFontFileName.c_str(), 12);

	if (font != 0) {
		fontsMap_[id] = font;
		return true;
	}

	return false;
}
//-------------------------------------------------------------------------------------------
bool
TextureManager::loadTextureFromRenderedText(std::string textureText, long fontId, long textureId, SDL_Color textColor, SDL_Renderer* pRenderer)
{
	//Render text surface
	TTF_Font* font = getFont(fontId);
	SDL_Surface* textSurface = TTF_RenderText_Solid(font, textureText.c_str(), textColor);
	SDL_Texture* pTexture = 0;
	if (textSurface == 0) {
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	} else {
		//Create texture from surface pixels
		pTexture = SDL_CreateTextureFromSurface(pRenderer, textSurface);
		if (pTexture == 0) {
			printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
		}
		//Get rid of preexisting texture
		if (textureMap_[textureId] != 0) {
			SDL_DestroyTexture(textureMap_[textureId]);
		}
		textureMap_[textureId] = pTexture;

		//Get rid of old surface
		SDL_FreeSurface(textSurface);
	}

	//Return success
	return (pTexture != 0);
}
//-------------------------------------------------------------------------------------------
SDL_Texture*
TextureManager::getTexture(long textureId)
{
	std::map<long, SDL_Texture*>::const_iterator it = textureMap_.find(textureId);
	if ( it != textureMap_.end() ) {
		return textureMap_[textureId];
	}
	return 0;

}
//-------------------------------------------------------------------------------------------
TTF_Font*
TextureManager::getFont(long fontId)
{
	std::map<long, TTF_Font*>::const_iterator it = fontsMap_.find(fontId);
	if (it != fontsMap_.end()) {
		return fontsMap_[fontId];
	}
	return 0;

}
//-------------------------------------------------------------------------------------------
void
TextureManager::render(long id, int x, int y, int width, int height, SDL_Renderer *pRenderer, SDL_RendererFlip flip)
{
	SDL_Rect srcRect;
	SDL_Rect destRect;
	srcRect.x = 0; srcRect.y = 0;
	srcRect.w = destRect.w = width;
	srcRect.h = destRect.h = height;
	destRect.x = x; destRect.y = y;
	SDL_RenderCopy(pRenderer, textureMap_[id], &srcRect, &destRect);
	//SDL_RenderCopyEx(pRenderer, textureMap_[id], &srcRect, &destRect, 0, 0, flip);
}
//-------------------------------------------------------------------------------------------
TextureManager::~TextureManager()
{
	for (auto it = textureMap_.begin(); it != textureMap_.end(); ) {
		SDL_DestroyTexture(it->second);
		textureMap_.erase(it++);
	}

	for (auto it = fontsMap_.begin(); it != fontsMap_.end(); ) {
		TTF_CloseFont(it->second);
		fontsMap_.erase(it++);
	}
}
//-------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------