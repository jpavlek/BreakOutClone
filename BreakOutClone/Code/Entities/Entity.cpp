#include "Entity.h"
#include "..\Graphics\TextureManager.h"
#include <iostream>
//-------------------------------------------------------------------------------------------
Entity::Entity()
{
}
//-------------------------------------------------------------------------------------------
Entity::Entity(char* entityName):
	entityName_(entityName)
{
}
//-------------------------------------------------------------------------------------------
Entity::Entity(char* entityName, int x, int y, int w, int h)
	: Entity(entityName, x, y, w, h, 0, 0, 0L, nullptr)
{
}
//-------------------------------------------------------------------------------------------
Entity::Entity(char* entityName, int x, int y, int w, int h, int velX, int velY, long textureId, SDL_Texture* texture)
:	entityName_(entityName),
	position_(SDL_Rect{ x, y, w, h }),
	velX_(velX),
	velY_(velY),
	textureId_(textureId),
	texture_(texture)
{
}
//-------------------------------------------------------------------------------------------
void
Entity::members()
{
	std::cout << "entityName_: " << entityName_ << std::endl;
	std::cout << "position_: (" << position_.x << ", " << position_.y << ", " << position_.w << ", " << position_.h << ")" << std::endl;
	std::cout << "velocity: (" << velX_ << ", " << velY_ << ")" << std::endl;
	std::cout << "textureId_: (" << textureId_ << ")" << std::endl;
	std::cout << "texture_: (" << texture_ << ")" << std::endl;
}
//-------------------------------------------------------------------------------------------
void
Entity::render(SDL_Renderer* sdlRenderer) {
	SDL_Rect sdl_rect = pos();
	TextureManager::Instance()->render(textureId_, position_.x, position_.y, position_.w, position_.h, sdlRenderer);
}
//-------------------------------------------------------------------------------------------
Entity::~Entity() {
	//No need to delete texture here, dynamic resources are dealocated through resource manager
}
//-------------------------------------------------------------------------------------------
bool
Entity::intersectsRectangle(Entity* entity)
{
	bool notIntersectsOnX = (position_.x > entity->position_.x + entity->position_.w) || (entity->position_.x > position_.x + position_.w);
	bool notIntersectsOnY = (position_.y > entity->position_.y + entity->position_.h) || (entity->position_.y > position_.y + position_.h);
	bool intersects = !(notIntersectsOnX || notIntersectsOnY);
	return intersects;
}
//-------------------------------------------------------------------------------------------
bool
Entity::operator < (const Entity& entity) const
{
	if (posY() > entity.posY()) {
		return true;
	}
	else if (posY() == entity.posY()) {
		return (posX() < entity.posX());
	}
	return false;
}
//-------------------------------------------------------------------------------------------
void
Entity::update()
{
}
//-------------------------------------------------------------------------------------------
// Returns rough relative positon of the ent object relative to the object on which the method is called.
// 0 1 2
// 3 4 5
// 6 7 8
// 0 = NW, 1 = N, 2 = NE, 3 = W, 4 = CENTER, 5 = E, 6 = SW, 7 = S, 8 = SE
int
Entity::relativePosition(Entity* ent)
{
	int centerX = (position_.x + position_.w) / 2;
	int centerY = (position_.y + position_.h) / 2;

	int entCenterX = (ent->posX() + ent->width()) / 2;
	int entCenterY = (ent->posY() + ent->height()) / 2;

	int relativePosX = entCenterX - centerX;
	int relativePosY = entCenterY - centerY;
	int x = 0;
	if (relativePosX > 0) {
		x = 2;
	}
	else if (relativePosX == 0) {
		x = 1;
	}

	int y = 0;
	if (relativePosY > 0) {
		y = 2;
	} else if (relativePosY == 0) {
		y = 1;
	}
	
	int result = 3 * x + y;
	return result;
}
//-------------------------------------------------------------------------------------------
void
Entity::playSoundEffect(long soundEffectId, AudioManager* audioManager) {
	if (audioManager == nullptr) {
		return;
	}
	audioManager->playSoundEffect(soundEffectId);
}
//-------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------