#pragma once

#include <SDL.h>
#include <string>

class Entity {
public:
	//ctors
	Entity();
	Entity(char* entityName);
	Entity(char* entityName, int x, int y, int w, int h);
	Entity(char* entityName, int x, int y, int w, int h, int velX, int velY, long textureId, SDL_Texture* texture);
	//Entity(const Entity& refEntity);
	//dtor
	virtual ~Entity();
	//getters
	char* entityName() const { return entityName_; }
	SDL_Rect pos() const { return position_; }
	int posX() const { return position_.x; }
	int posY() const { return position_.y; }
	int width() { return position_.w; }
	int height() const { return position_.h; }
	int velX() const { return velX_; }
	int velY() const { return velY_; }
	int centerX() const { return (position_.x + position_.w) / 2; }
	int centerY() const { return (position_.y + position_.h) / 2; }
	SDL_Texture* getTexture() const { return texture_; }
	void members();
	//setters
	void posX(int x) { position_.x = x; }
	void posY(int y) { position_.y = y; }
	void pos(int x, int y) { position_.x = x; position_.y = y; }
	void width(int w) { position_.w = w; }
	void height(int h) { position_.h = h; }
	void dimensions(int w, int h) { position_.w = w; position_.h = h; }
	void initPosition(int x, int y, int w, int h) { position_.x = x; position_.y = y; position_.w = w; position_.h = h; }
	void velX(int x) { velX_ = x; }
	void velY(int y) { velY_ = y; }
	void initVel(int x, int y) { velX_ = x; velY_ = y; }
	void setSDLTexture(SDL_Texture* imageTexture) { texture_ = imageTexture; }
	virtual void render(SDL_Renderer* sdlRenderer);
	virtual bool intersectsRectangle(Entity* entity);
	virtual void update();
	bool operator < (const Entity& entity) const;
	int relativePosition(Entity* ent);
protected:
	
	//data member defaults
	char* entityName_{ "Entity" };
	SDL_Rect position_{ 0, 0, 0, 0 };
	int velX_{ 0 };
	int velY_{ 0 };
	long textureId_{ 0 };
	SDL_Texture* texture_{ NULL };
};