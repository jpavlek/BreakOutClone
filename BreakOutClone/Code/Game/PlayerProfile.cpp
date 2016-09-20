#include "PlayerProfile.h"
#include "..\Entities\Brick.h"

//-------------------------------------------------------------------------------------------
PlayerProfile::PlayerProfile(int score, int lives):
	score_(score),
	lives_(lives)
{
}
//-------------------------------------------------------------------------------------------
PlayerProfile::~PlayerProfile()
{
}
//-------------------------------------------------------------------------------------------
void
PlayerProfile::updateScore(std::vector<Entity*> &ent)
{
	for (size_t i = 0; i < ent.size(); i++) {
		Brick* brick = dynamic_cast<Brick*> (ent[i]);
		if (brick == nullptr || brick->id() == 'I') {
			continue;
		}
		if (brick->pointsLeft() <= 0) {
			brick->playBreakSound();
			addScore(brick->breakScore());
		}
	}
}
//-------------------------------------------------------------------------------------------
void
PlayerProfile::setInitialConditions(int score, int lives)
{
	setScore(score);
	setLives(lives);
}
//-------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------