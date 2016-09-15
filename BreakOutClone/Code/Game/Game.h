#pragma once

#include "..\InputEvents\InputEventHandler.h"
#include "..\Graphics\Graphics.h"
#include "..\Graphics\SDLGraphics.h"
#include "..\Entities\Paddle.h"
#include "..\Entities\Ball.h"
#include "..\ResourceManager\ResourceManager.h"
#include "..\Timer\Timer.h"
#include "..\Graphics\TextureDescription.h"
#include "..\Graphics\FontDescription.h"
#include "..\ResourceManager\AudioDescription.h"
#include "..\Entities\BrickType.h"
#include "..\Entities\BrickWall.h"
#include "..\Collision\QuadTree.h"
#include "PlayerProfile.h"

enum class GameState
{
	GAME_OVER = -2,
	LIFE_LOST = -1,
	WAIT_TO_START = 0,
	GAME_PLAY,
	NEXT_LEVEL,
	GAME_WON
};

class Game
{
public:

	Game();
	~Game();
	
	// getters
	bool running() { return running_; }
	ResourceManager* resourceManager() { return resourceManager_; }
	Paddle* paddle() { return paddle_; }
	Ball* ball() { return ball_; }

	void init();	
	void initEntities();
	void update();
	void handleEvents();
	void render();
	void clean();
	void loop();

	void printMembers();
	void run();
	void loadLevel(int level);
	TextureDescription createTextureThroughResourceManager(std::string fullPathImageFileName);
	FontDescription createFontThroughResourceManager(std::string fullPathFontFileName);
	TextureDescription createTextureFromRenderedTextThroughResourceManager(std::string textureText, long fontId, SDL_Color textColor);
	AudioDescription createSoundEffectThroughResourceManager(std::string fullPathSFXFileName);
	void handleCollisions();
	void checkForDestroyedBricks();
	PlayerProfile* playerProfile() { return playerProfile_; }
	void setGameState(GameState state) { gameState_ = state; }
	void onLifeLost();
	void updateHUD();
	bool initSDLSoundMixer();
	bool loadSoundEffect(std::string soundEffectPathFileName);
	void nextLevel();
	void clearLevel();
	bool checkLevelFinished();
private:
	bool running_{ false };
	Graphics* graphics_{ 0 };
	InputEventHandler* inputHandler_{ 0 };
	ResourceManager* resourceManager_{ 0 };
	Paddle* paddle_{ 0 };
	Ball* ball_{ 0 };
	Timer* timer_{ 0 };
	std::map<char, BrickType> brickTypesMap_;
	std::vector<Brick*> bricks_;
	BrickWall* brickWall_{ 0 };
	QuadTree* quadTree_{ 0 };
	TreeNode* ballCollisionLocationNode_{ 0 };
	PlayerProfile* playerProfile_{ 0 };
	GameState gameState_{ GameState::WAIT_TO_START };
	Brick* livesText_{ 0 };
	Brick* scoreText_{ 0 };
	FontDescription fontDescription_{ 0, 0 };
	TextureDescription livesTextureDesc_{ 0, 0 };
	TextureDescription scoreTextureDesc_{ 0, 0 };
	SDL_Color textColor_{ 0, 0, 0, 0 };
	std::map<long, Mix_Chunk*> soundEffectsMap_;
	int level_{ 1 };
	int maxLevel_{ 3 };
};

