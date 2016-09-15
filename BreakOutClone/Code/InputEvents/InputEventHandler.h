#pragma once
class Game;

class InputEventHandler {
public:
	InputEventHandler(Game* game);
	InputEventHandler();
	virtual bool handleInputEvents() = 0;
	Game* game() { return game_; }
	virtual ~InputEventHandler() {};
private:
	Game* game_;
};
