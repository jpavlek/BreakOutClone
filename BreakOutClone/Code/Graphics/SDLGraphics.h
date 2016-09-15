#pragma once
#include "Graphics.h"

/*struct SDL_Surface;
struct SDL_Window;
struct SDL_Renderer;*/

#include <SDL.h>
class SDLGraphics: public Graphics
{
public:
	//inherited Graphics methods:
	bool initGraphics() override;
	bool closeGraphics() override;
	bool unloadMedia(TextureManager* textureManager) override;
	void setBackgroundColor(int r, int g, int b, int alpha) override;
	
	// SDLGraphics own methods:
	SDLGraphics();
	~SDLGraphics();
	bool initSDL();
	bool unloadSDLMedia(TextureManager* textureManager);
	bool closeSDL();
	SDL_Renderer* sdlRenderer() { return sdlRenderer_; }

	//Loads individual image
	SDL_Texture* loadTexture(std::string path);
private:
	SDL_Window* sdlWindow_;
	SDL_Renderer* sdlRenderer_;
};