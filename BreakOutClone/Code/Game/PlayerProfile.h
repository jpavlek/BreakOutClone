#pragma once
#include <vector>
#include <SDL.h>
#include <SDL_ttf.h>
class Entity;

class PlayerProfile
{
public:
	PlayerProfile(int score, int lives);
	~PlayerProfile();
	int score() { return score_; }
	int lives() { return lives_; }
	void addScore(int scoreToAdd) { score_ += scoreToAdd; }
	void decreaseLives() { lives_ --; }
	void updateScore(std::vector<Entity*> &ent);
	void setScore(int score) { score_ = score; }
	void setLives(int lives) { lives_ = lives; }
	void setInitialConditions(int score = 0, int lives = 3);
private:
	int score_{ 0 };
	int lives_{ 0 };
};