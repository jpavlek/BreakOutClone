#pragma once

#include "Direction.h"
#include "Paddle.h"
#include "Brick.h"
#include "BrickWall.h"
#include <SDL.h>
#include "BallDescription.h"

class Game;
class QuadTree;

//-------------------------------------------------------------------------------------------
class Ball: public Entity {
public:
	Ball();
	Ball(int x, int y, int diameter, int velX, int velY, long textureId, SDL_Texture* sdlTexture);
	Ball(BallDescription* ballDesc);
	Ball(int x, int y, int diameter);
	void setOwner(Game* owner) { owner_ = owner; }
	void move();
	void increasePosX(int deltaX) { position_.x += deltaX; }
	void increasePosY(int deltaY) { position_.y += deltaY; }
	void setInitialConditions(int posX, int posY, int velX, int velY);
	//void setDefaultConditions();
	
	void handleWallCollisions();
	bool checkWallCollision(Entity* entity, Direction dir);
	
	void handlePaddleCollision(Paddle* paddle);
	bool checkPaddleCollision(Paddle* paddle);
	void resolvePaddleCollision(int distanceXFromPaddleCenter);
	
	bool checkBrickWallCollision(BrickWall* brickWall);
	bool checkBrickCollision(Brick* brick);
	void resolveBrickWallCollision(std::vector<Entity*> &ent);
	void resolveBrickCollision(Brick* brick);
	
	void setDiameter(int diameter);
	virtual ~Ball();
	//int min(int a, int b);
	//int max(int a, int b);
private:
	int diameter_{ 0 };
	Game* owner_{ 0 };
	
};
//-------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------