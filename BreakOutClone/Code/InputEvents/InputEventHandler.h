#pragma once

class Game;

class InputEventHandler {
public:
	InputEventHandler(Game* game);
	InputEventHandler();
	virtual ~InputEventHandler() {};
	Game* game() { return game_; }
	virtual bool handleInputEvents() = 0;
	
private:
	Game* game_;
};
