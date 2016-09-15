#include "SDLGraphics.h"
#include <SDL_image.h>
#include "GraphicsProperties.h"
#include <vector>

//-------------------------------------------------------------------------------------------
/* SDLGraphics methods: */
//-------------------------------------------------------------------------------------------
SDLGraphics::SDLGraphics()
	: Graphics("SDLGraphics"),
	sdlWindow_(0),
	sdlRenderer_(0)
{
}
//-------------------------------------------------------------------------------------------
SDLGraphics::~SDLGraphics()
{
	closeSDL();
}
//-------------------------------------------------------------------------------------------
/* Initialize SDL */
//-------------------------------------------------------------------------------------------
bool
SDLGraphics::initSDL()
{
	bool SDL_init_status = false;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return SDL_init_status;
	}

	//Create SDLwindow
	sdlWindow_ = SDL_CreateWindow("Breakout Clone", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	
	if (sdlWindow_ == NULL) {
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		return SDL_init_status;
	}

	//Initialize PNG loading
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags)) {
		printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
		SDL_init_status = false;
	}
	else {
		sdlRenderer_ = SDL_CreateRenderer(sdlWindow_, -1, 0);
		SDL_init_status = true;
	}

	if (TTF_Init() == -1) {
		printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
		SDL_init_status = false;
	}

	return SDL_init_status;
}
//-------------------------------------------------------------------------------------------
SDL_Texture*
SDLGraphics::loadTexture(std::string path)
{
	SDL_Texture* newTexture = NULL;

	SDL_Surface* loadedSurface = IMG_Load(path.c_str());

	if (loadedSurface == NULL) {
		printf("Unable to load image %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		return newTexture;
	}

	newTexture = SDL_CreateTextureFromSurface(sdlRenderer_, loadedSurface);
	if (newTexture == NULL)
	{
		printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
	}

	//Get rid of old loaded surface
	SDL_FreeSurface(loadedSurface);

	return newTexture;
}
//-------------------------------------------------------------------------------------------
//close SDL
bool
SDLGraphics::closeSDL()
{
	//Destroy window
	SDL_DestroyWindow(sdlWindow_);
	sdlWindow_ = NULL;

	SDL_DestroyRenderer(sdlRenderer_);
	sdlRenderer_ = NULL;

	//Quit SDL subsystems
	SDL_Quit();

	return true;
}
//-------------------------------------------------------------------------------------------
bool
SDLGraphics::unloadSDLMedia(TextureManager* textureManager) {

	bool sdlMediaUnloaded = false;
	std::map< long, SDL_Texture* >::iterator it = textureManager->textureMap()->begin();
	
	// Destroy textures
	for ( ; it != textureManager->textureMap()->end(); it++) {
		SDL_Texture* texture = it->second;		
		SDL_DestroyTexture(texture);
	}

	//Empty texture map
	it = textureManager->textureMap()->begin();
	textureManager->textureMap()->erase(it, textureManager->textureMap()->end());
	sdlMediaUnloaded = true;

	return sdlMediaUnloaded;
}
//-------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------
/* Inherited Graphics methods: initGraphics, closeGraphics, loadMedia,  */
//-------------------------------------------------------------------------------------------
bool
SDLGraphics::initGraphics()
{
	bool sdlGraphicsInited = initSDL();

	if (!sdlGraphicsInited) {
		printf("Failed to initialize SDL! [SDLGraphics::initGraphics()]\n");
	} else {
		printf("SDL Graphics initialized! [SDLGraphics::initGraphics()]\n");
	}

	return sdlGraphicsInited;
}
//-------------------------------------------------------------------------------------------
bool
SDLGraphics::closeGraphics()
{
	bool graphicsClosed = closeSDL();
	if (graphicsClosed) {
		printf("Graphics closed! [SDLGraphics::closeGraphics()]\n");
	}

	return graphicsClosed;
}
//-------------------------------------------------------------------------------------------
bool
SDLGraphics::unloadMedia(TextureManager* textureManager) {

	bool sdlMediaLoaded = unloadSDLMedia(textureManager);
	//Load media
	if (!sdlMediaLoaded) {
		printf("Failed to load media! [SDLGraphics::loadMedia()]\n");
	}
	else {
		printf("Media loaded successfully! [SDLGraphics::loadMedia()]\n");
	}

	return sdlMediaLoaded;
}
//-------------------------------------------------------------------------------------------
void
SDLGraphics::setBackgroundColor(int r, int g, int b, int alpha)
{
	SDL_SetRenderDrawColor(sdlRenderer_, r, g, b, alpha);
}
//-------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------
