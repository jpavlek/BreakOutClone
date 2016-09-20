#pragma once

#include "InputEventHandler.h"
#include "..\Game\Game.h"

class SDLInputEventHandler : public InputEventHandler
{
public:
	//inherited InputEventHandler methods:
	bool handleInputEvents() override;
	SDLInputEventHandler(Game* game);
	~SDLInputEventHandler();
};