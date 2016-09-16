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
Entity::Entity(int x, int y, int w, int h)
	: Entity("EntityName", x, y, w, h, 0, 0, 0L, NULL)
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
//Copy constructor not needed, default shallow copy constructor used instead for better reusing of SDL surfaces, SDL surface resources deleted in main() function
//TODO: Delete copy constructor
//Entity::Entity(const Entity& refEntity)
//	: Entity(refEntity.position.x, refEntity.position.y, refEntity.position.w, refEntity.position.h, refEntity.velX(), refEntity.velY(), NULL)
//{
//	texture = new SDL_Surface();
//	*texture = *refEntity.getTexture();
//}
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
//The same applies for destructor, no need to delete texture, dynamic resources are dealocated in main()
Entity::~Entity() {
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
//-------------------------------------------------------------------------------------------