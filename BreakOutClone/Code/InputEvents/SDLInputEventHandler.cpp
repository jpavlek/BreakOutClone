#include "SDLInputEventHandler.h"
#include <SDL.h>
#include "..\Game\Game.h"
#include "..\Entities\Paddle.h"

//-------------------------------------------------------------------------------------------
SDLInputEventHandler::SDLInputEventHandler(Game* game)
	:InputEventHandler(game)
{
	
}
//-------------------------------------------------------------------------------------------
SDLInputEventHandler::~SDLInputEventHandler() {

}
//-------------------------------------------------------------------------------------------
bool
SDLInputEventHandler::handleInputEvents() {
	//Event handler
	SDL_Event sdl_event;
	Paddle* paddle = game()->paddle();

	//Handle events on queue
	while (SDL_PollEvent(&sdl_event) != 0) {
		//User requests quit
		if (sdl_event.type == SDL_QUIT) {
			return true;
		}

		//User presses a key
		if (sdl_event.type == SDL_KEYDOWN) {
			//Select surfaces based on key press
			switch (sdl_event.key.keysym.sym) {
			case SDLK_UP:
				break;

			case SDLK_DOWN:
				break;

			case SDLK_LEFT:
				paddle->setMaxVelocityLeft();
				break;

			case SDLK_RIGHT:
				paddle->setMaxVelocityRight();
				break;

			default:
				break;
			}
		}

		if (sdl_event.type == SDL_MOUSEMOTION || sdl_event.type == SDL_MOUSEBUTTONDOWN || sdl_event.type == SDL_MOUSEBUTTONUP) {
			//Get mouse position
			int x, y;
			SDL_GetMouseState(&x, &y);
			paddle->allignWithMousePosition(x);
		}

		return false;
	}

	return false;
}
//-------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------