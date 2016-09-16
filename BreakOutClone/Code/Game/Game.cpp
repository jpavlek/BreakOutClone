#include "Game.h"
#include "..\Graphics\SDLGraphics.h"
#include "..\InputEvents\SDLInputEventHandler.h"
#include "..\Entities\BallDescription.h"
#include "..\Entities\Ball.h"
#include "..\ResourceManager\ResourceManager.h"
#include <iostream>
#include "..\..\..\..\TinyXML\tinyxml.h"
#include "..\Entities\BrickType.h"
#include "..\Graphics\GraphicsProperties.h"
#include "..\Entities\BrickWall.h"
#include "..\Entities\Entity.h"
#include <vector>
#include <algorithm>

#define SDL_GRAPHICS 1
//-------------------------------------------------------------------------------------------
Game::Game()
	: timer_( new Timer() ),
	playerProfile_( new PlayerProfile(0, 3) ),
#ifdef SDL_GRAPHICS
	graphics_( new SDLGraphics() ),
	inputHandler_ (new SDLInputEventHandler(this) ),
	resourceManager_( new ResourceManager() )
#else
	graphics_(0),
	inputHandler_(0),
	resourceManager_(0)
#endif // SDL_GRAPHICS
{
}
//-------------------------------------------------------------------------------------------
Game::~Game()
{
	delete playerProfile_;
	delete quadTree_;
	
	delete brickWall_;
	
	for (size_t i = 0; i < bricks_.size(); i++) {
		delete bricks_[i];
	}
	bricks_.clear();
	delete timer_;
	delete paddle_;
	delete ball_;
	delete livesText_;
	delete scoreText_;
	delete background_;
	delete inputHandler_;
	delete resourceManager_;
	delete graphics_;	
}
//-------------------------------------------------------------------------------------------
void
Game::init() {
	running_ = true;

	bool graphicsLoaded = false;
	
	//init Graphics
	if (graphics_) {
		graphicsLoaded = graphics_->initGraphics();
	}

	bool soundEffectsLoaded = initSDLSoundMixer();

	//TODO: load sound effects	soundEffectsMap_[1] = 

	graphics_->setBackgroundColor(248, 248, 248, 255);

	if (graphicsLoaded) {
		initEntities();
	}

	playerProfile_->setInitialConditions();
	setGameState(GameState::GAME_PLAY);
}
//-------------------------------------------------------------------------------------------
void
Game::clean()
{
	//Cleaning game components in reverse order from init
	gameObjects_.clear();

	bool mediaUnloaded = false;
	bool graphicsClosed = false;
	
	if (graphics_) {
		TextureManager* textureManager = resourceManager_->textureManager();
		mediaUnloaded = graphics_->unloadMedia(textureManager);
		graphicsClosed = graphics_->closeGraphics();
	}	
}
//-------------------------------------------------------------------------------------------
void
Game::render() {
	SDLGraphics* sdlGraphics = dynamic_cast<SDLGraphics*> (graphics_);
	SDL_Renderer* sdlRenderer = sdlGraphics->sdlRenderer();
	SDL_RenderClear(sdlRenderer); // clear the renderer to the draw color
	
	for (size_t i = 0; i < gameObjects_.size(); i++) {
		Entity* object = gameObjects_[i];
		if (object == 0) {
			continue;
		}
		object->render(sdlRenderer);
	}
	quadTree_->render(sdlRenderer);
	if (ballCollisionLocationNode_) {
		ballCollisionLocationNode_->debugRender(sdlRenderer, 255, 0, 0, 255);
	}
	SDL_RenderPresent(sdlRenderer); // draw to the screen 
}
//-------------------------------------------------------------------------------------------
void
Game::handleEvents() {

	bool quit = false;
	if (inputHandler_) {
		quit = inputHandler_->handleInputEvents();
		if (quit) {
			running_ = false;
		}
	}
}
//-------------------------------------------------------------------------------------------
void
Game::initEntities() {

	TextureDescription ballTextureDesc = createTextureThroughResourceManager("Resources/Images/Ball.png");
	TextureDescription paddleTextureDesc = createTextureThroughResourceManager("Resources/Images/Paddle.png");

	BallDescription ballDesc;
	ballDesc.setDefaultValues();
	ballDesc.setTexture(ballTextureDesc.texture_);
	ballDesc.setTextureId(ballTextureDesc.textureId_);
	ball_ = new Ball(&ballDesc);
	ball_->setOwner(this);

	PaddleDescription paddleDesc;
	paddleDesc.setDefaultValues();
	paddleDesc.setTexture(paddleTextureDesc.texture_);
	paddleDesc.setTextureId(paddleTextureDesc.textureId_);
	paddle_ = new Paddle(&paddleDesc);

	fontDescription_ = createFontThroughResourceManager("Resources/Font/NotoMono-Regular.ttf");
	
	textColor_ = { 63, 127, 255, 255 };

	livesTextureDesc_ = createTextureFromRenderedTextThroughResourceManager("Lives: " + std::to_string(playerProfile_->lives()), fontDescription_.fontId_, textColor_);
	scoreTextureDesc_ = createTextureFromRenderedTextThroughResourceManager("Score: " + std::to_string(playerProfile_->score()), fontDescription_.fontId_, textColor_);
	
	livesText_ = new Entity("Lives", 500, 420, 120, 15, 0, 0, livesTextureDesc_.textureId_, livesTextureDesc_.texture_);
	scoreText_ = new Entity("Score", 20, 420, 120, 15, 0, 0, scoreTextureDesc_.textureId_, scoreTextureDesc_.texture_);

	SDL_Rect quadTreeBoundaries = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	quadTree_ = new QuadTree(quadTreeBoundaries, 4);
	loadLevel(level_);
	addGameObject(background_);
	addGameObject(ball_);
	addGameObject(paddle_);
	addGameObject(brickWall_);
	addGameObject(livesText_);
	addGameObject(scoreText_);

}
//-------------------------------------------------------------------------------------------
void
Game::update()
{
	switch (gameState_)
	{
	case (GameState::GAME_PLAY):
		//paddle_->move();
		//ball_->move();
		for (size_t i = 0; i < gameObjects_.size(); i++) {
			Entity* object = gameObjects_[i];
			if (object == 0) {
				continue;
			}
			object->update();
		}
		handleCollisions();
		break;
	case (GameState::WAIT_TO_START):
		ball_->setInitialConditions((SCREEN_WIDTH - BALL_DIAMETER) / 2, (SCREEN_HEIGHT - BALL_DIAMETER) / 2, BALL_SPEED, BALL_SPEED);
		ball_->setDiameter(BALL_DIAMETER);
		setGameState(GameState::GAME_PLAY);
		SDL_Delay(1000);
		break;
	case (GameState::LIFE_LOST):
		onLifeLost();
		break;
	case (GameState::NEXT_LEVEL):
		nextLevel();
		break;
	case (GameState::GAME_OVER):
		break;
	default:
		break;
	}
}
//-------------------------------------------------------------------------------------------
void
Game::printMembers()
{
	std::cout << "running_: " << running_ << std::endl;
	std::cout << "graphics_: " << graphics_ << std::endl;
	std::cout << "inputHandler_: " << inputHandler_ << std::endl;
	std::cout << "resourceManager_: " << resourceManager_ << std::endl;
	std::cout << "paddle_: " << paddle_ << std::endl;
	std::cout << "ball_: " << ball_ << std::endl;
}
//-------------------------------------------------------------------------------------------
void
Game::loop()
{
	while (running()) {
		timer_->markFrameStart();
		handleEvents();
		update();
		render();
		timer_->adjustToFrameRate();
	}
}
//-------------------------------------------------------------------------------------------
void
Game::run()
{
	init();
	loop();
	clean();
}
//-------------------------------------------------------------------------------------------
void
Game::loadLevel(int level) {
	std::string fileName = "Resources/Levels/Level" + std::to_string(level) + ".xml";
	const char* pFileName = fileName.c_str();
	TiXmlDocument doc(fileName.c_str());
	bool loadOK = doc.LoadFile();
	if (!loadOK) {
		printf("Failed to load file \"%s\"\n", pFileName);
		return;
	}

	TiXmlHandle hDoc(&doc);
	TiXmlHandle hRoot(0);

	TiXmlElement* pElem = hDoc.FirstChildElement().Element();
	// should always have a valid root but handle gracefully if it doesn't
	if (!pElem) {
		printf("No root element found in file \"%s\"\n", pFileName);
		return;
	}

	// save this for later
	hRoot = TiXmlHandle(pElem);
	std::string rootName = pElem->Value();
	int rows = 0;
	int columns = 0;
	int rowSpacing = 1;
	int columnSpacing = 1;
	if (rootName == "Level") {
		pElem->QueryIntAttribute("RowCount", &rows); // If this fails, original value is left as-is
		pElem->QueryIntAttribute("ColumnCount", &columns);
		pElem->QueryIntAttribute("RowSpacing", &rowSpacing);
		pElem->QueryIntAttribute("ColumnSpacing", &columnSpacing);
		const char* texturePathName = readStringAttribute(pElem, "BackgroundTexture");
		TextureDescription backgroundTextureDesc = createTextureThroughResourceManager(texturePathName);
		background_ = new Entity("Background", 0, 0, 640, 480, 0, 0, backgroundTextureDesc.textureId_, backgroundTextureDesc.texture_);
	}

	TiXmlElement* pBrickTypesElem = hRoot.FirstChild("BrickTypes").Element();

	if (pBrickTypesElem) {
		pElem = hRoot.FirstChild("BrickTypes").FirstChild("BrickType").Element();
	}
	
	//read and create brick types
	int counter = 0;
	for (pElem; pElem; pElem = pElem->NextSiblingElement()) {
		const char *pKey = pElem->Value();
		const char* id = readStringAttribute(pElem, "Id");
		char idChar = id[0];
		const char* texturePathName = readStringAttribute(pElem, "Texture");
		int hitPoints = 0;
		pElem->QueryIntAttribute("HitPoints", &hitPoints);
		const char* hitSoundPath = readStringAttribute(pElem, "HitSound");
		const char* breakSoundPath = readStringAttribute(pElem, "BreakSound");
		
		int breakScore = 0;
		pElem->QueryIntAttribute("BreakScore", &breakScore);
		
		TextureDescription brickTypeTextureDesc = createTextureThroughResourceManager(texturePathName);
		AudioDescription hitSoundDesc = createSoundEffectThroughResourceManager(hitSoundPath);
		AudioDescription breakSoundDesc = createSoundEffectThroughResourceManager(breakSoundPath);

		BrickType brickType(idChar, texturePathName, hitPoints, hitSoundPath, breakSoundPath, breakScore, brickTypeTextureDesc.texture_, brickTypeTextureDesc.textureId_, hitSoundDesc, breakSoundDesc);

		brickTypesMap_[idChar] = brickType;
		counter++;
	}

	//read and create bricks
	TiXmlElement* pBricksElem = hRoot.FirstChild("Bricks").Element();
	TiXmlText* text = hDoc.ChildElement("Level", 0).ChildElement("Bricks", 0).FirstChild().Text();
	if (text) {
		const char* brickWall = text->Value();
		std::string brickWallString = brickWall;

		int offsetX = -1;
		int brickCounter = -1;
		int miss = 0;
		SDL_Texture* brickTexture = NULL;
		Mix_Chunk* brickHitSound = NULL;
		Mix_Chunk* brickBrokenSound = NULL;
		bricks_.resize(brickWallString.size());
		SDL_Rect wallBoundaries{ -1, -1, -1, -1 };
		int destroyableBricks = 0;
		for (size_t i = 0; i < brickWallString.size(); i++) {
			char brickTypeId = brickWall[i];
			switch (brickTypeId)
			{
			case '_':
				offsetX++;
				continue;
				break;
			case 'H':
				offsetX++;
				brickCounter++;
				brickTexture = brickTypesMap_[brickTypeId].texture_;
				brickHitSound = brickTypesMap_[brickTypeId].hitSound.soundEffect_;
				brickBrokenSound = brickTypesMap_[brickTypeId].breakSound.soundEffect_;
				break;
			case 'M':
				offsetX++;
				brickCounter++;
				brickTexture = brickTypesMap_[brickTypeId].texture_;
				brickHitSound = brickTypesMap_[brickTypeId].hitSound.soundEffect_;
				brickBrokenSound = brickTypesMap_[brickTypeId].breakSound.soundEffect_;
				break;
			case 'S':
				offsetX++;
				brickCounter++;
				brickTexture = brickTypesMap_[brickTypeId].texture_;
				brickHitSound = brickTypesMap_[brickTypeId].hitSound.soundEffect_;
				brickBrokenSound = brickTypesMap_[brickTypeId].breakSound.soundEffect_;
				break;
			case 'I':
				offsetX++;
				brickCounter++;
				brickTexture = brickTypesMap_[brickTypeId].texture_;
				brickBrokenSound = brickTypesMap_[brickTypeId].breakSound.soundEffect_;
				break;
			default:
				miss++;
				continue;
				break;
			}
			/*if (brickTypeId == '_') {
				continue;
			}*/
			int x = ( offsetX % columns ) * BRICK_WIDTH;
			int y = ( offsetX / columns ) * BRICK_HEIGHT;
			int w = BRICK_WIDTH;
			int h = BRICK_HEIGHT;
			if (x < wallBoundaries.x || wallBoundaries.x < 0) {
				wallBoundaries.x = x;
			}
			if (y < wallBoundaries.y || wallBoundaries.y < 0) {
				wallBoundaries.y = y;
			}
			if (x + BRICK_WIDTH > wallBoundaries.w || wallBoundaries.w < 0) {
				wallBoundaries.w = x + BRICK_WIDTH;
			}
			if (y + BRICK_HEIGHT > wallBoundaries.h || wallBoundaries.h < 0) {
				wallBoundaries.h = y + BRICK_HEIGHT;
			}

			const char* txtureName = brickTypesMap_[brickTypeId].texturePathName_;
			const char* hitSndName = brickTypesMap_[brickTypeId].hitSoundPathName_;
			const char* breakSndName = brickTypesMap_[brickTypeId].breakSoundPathName_;
			int hitPnts = brickTypesMap_[brickTypeId].hitPoints_;
			int breakScr = brickTypesMap_[brickTypeId].breakScore_;
			if (brickTypeId != 'I') {
				destroyableBricks++;
			}
			Brick* brick = new Brick(x, y, BRICK_WIDTH, BRICK_HEIGHT, brickTypeId, txtureName, hitSndName, breakSndName, hitPnts, breakScr, brickTypesMap_[brickTypeId].textureId_, brickTexture, brickHitSound, brickBrokenSound);
			bricks_[brickCounter] = brick;
		}
		printf("Number of brick rows: %d\n", miss);
		bricks_.resize(brickCounter + 1);
		brickWall_ = new BrickWall(bricks_, wallBoundaries, destroyableBricks);
		quadTree_->insertBricks(bricks_);
	}
}
//-------------------------------------------------------------------------------------------
TextureDescription
Game::createTextureThroughResourceManager(std::string fullPathImageFileName)
{
	SDLGraphics* sdlGraphics = dynamic_cast<SDLGraphics*> (graphics_);
	SDL_Renderer* sdlRenderer = sdlGraphics->sdlRenderer();

	long textureId = resourceManager_->addTexture(fullPathImageFileName, sdlRenderer);
	SDL_Texture* texture = resourceManager_->getTexture(textureId);

	return TextureDescription(texture, textureId);
}
//-------------------------------------------------------------------------------------------
TextureDescription
Game::createTextureFromRenderedTextThroughResourceManager(std::string textureText, long fontId, SDL_Color textColor)
{
	SDLGraphics* sdlGraphics = dynamic_cast<SDLGraphics*> (graphics_);
	SDL_Renderer* sdlRenderer = sdlGraphics->sdlRenderer();

	long textureId = resourceManager_->addTextureFromFontRenderedText(textureText, fontId, textColor, sdlRenderer);
	SDL_Texture* texture = resourceManager_->getTexture(textureId);

	return TextureDescription(texture, textureId);
}
//-------------------------------------------------------------------------------------------
FontDescription
Game::createFontThroughResourceManager(std::string fullPathFontFileName)
{
	SDLGraphics* sdlGraphics = dynamic_cast<SDLGraphics*> (graphics_);
	SDL_Renderer* sdlRenderer = sdlGraphics->sdlRenderer();

	long fontId = resourceManager_->addFont(fullPathFontFileName, sdlRenderer);
	TTF_Font* font = resourceManager_->getFont(fontId);
	
	if (font == 0) {
		printf("Failed to load the font! SDL_ttf Error: %s\n", TTF_GetError());
	}

	return FontDescription(font, fontId);
}
//-------------------------------------------------------------------------------------------
AudioDescription
Game::createSoundEffectThroughResourceManager(std::string fullPathSFXFileName)
{
	AudioManager* audioManager = resourceManager_->audioManager();
	
	long sfxId = resourceManager_->addSoundEffect(fullPathSFXFileName);
	Mix_Chunk* sfx = resourceManager_->getSoundEffect(sfxId);

	if (sfx == 0) {
		printf("Failed to load sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
	}

	return AudioDescription(sfx, sfxId);
}
//-------------------------------------------------------------------------------------------
void
Game::handleCollisions()
{	
}
//-------------------------------------------------------------------------------------------
void
Game::checkForDestroyedBricks()
{
	brickWall_->checkForDestroyedBricks();
	quadTree_->checkForDestroyedElements();
}
//-------------------------------------------------------------------------------------------
void
Game::onLifeLost()
{
	playerProfile_->decreaseLives();
	updateHUD();
	if (playerProfile_->lives() <= 0) {
		setGameState(GameState::GAME_OVER);
	} else {
		setGameState(GameState::WAIT_TO_START);
	}
}
//-------------------------------------------------------------------------------------------
void
Game::updateHUD()
{
	SDLGraphics* sdlGraphics = dynamic_cast<SDLGraphics*> (graphics_);
	SDL_Renderer* sdlRenderer = sdlGraphics->sdlRenderer();

	livesTextureDesc_.texture_ = resourceManager_->updateTextureFromFontRenderedText("Lives: " + std::to_string(playerProfile_->lives()), fontDescription_.fontId_, livesTextureDesc_.textureId_, textColor_, sdlRenderer);
	scoreTextureDesc_.texture_ = resourceManager_->updateTextureFromFontRenderedText("Score: " + std::to_string(playerProfile_->score()), fontDescription_.fontId_, scoreTextureDesc_.textureId_, textColor_, sdlRenderer);

	livesText_->setSDLTexture(livesTextureDesc_.texture_);
	scoreText_->setSDLTexture(scoreTextureDesc_.texture_);
}
//-------------------------------------------------------------------------------------------
bool
Game::initSDLSoundMixer()
{
	bool sdlMixerLoaded = true;
	//Initialize SDL_mixer
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
		sdlMixerLoaded = false;
	}
	return sdlMixerLoaded;
}
//-------------------------------------------------------------------------------------------
bool
Game::loadSoundEffect(std::string soundEffectPathFileName)
{
	bool soundEffectFileLoaded = true;
	Mix_Chunk* soundEffect = Mix_LoadWAV( soundEffectPathFileName.c_str() );
	if (soundEffect == 0) {
		printf("Failed to load low sound effect! SDL_mixer Error: %s\n", Mix_GetError());
		soundEffectFileLoaded = false;
	}
	return soundEffectFileLoaded;
}
//-------------------------------------------------------------------------------------------
void
Game::nextLevel()
{
	level_++;
	if (level_ < maxLevel_) {
		clearLevel();
		loadLevel(level_);
	} else {
		setGameState(GameState::GAME_WON);
	}
}
//-------------------------------------------------------------------------------------------
void
Game::clearLevel()
{
	quadTree_->clear();
	delete brickWall_;
	brickWall_ = 0;
	for (int i = bricks_.size() - 1; i > 0; i--) {
		delete bricks_[i];
		bricks_[i] = 0;
		bricks_.erase(bricks_.begin() + i);
	}
}
//-------------------------------------------------------------------------------------------
bool
Game::checkLevelFinished()
{
	if (brickWall_->destroyableBricksCount()) {
		return false;
	}
	return true;
}
//-------------------------------------------------------------------------------------------
void
Game::addGameObject(Entity* entity)
{
	gameObjects_.push_back(entity);
}
//-------------------------------------------------------------------------------------------
void
Game::removeGameObject(Entity* entity)
{
	std::vector<Entity*>::iterator it = gameObjects_.begin();
	it = std::find(gameObjects_.begin(), gameObjects_.end(), entity);
	int nPosition = std::distance(gameObjects_.begin(), it);
	if (nPosition > 0) {
		gameObjects_.erase(gameObjects_.begin() + nPosition);
	}
}
//-------------------------------------------------------------------------------------------
void
Game::clearGameObjects()
{
	gameObjects_.clear();
}
//-------------------------------------------------------------------------------------------
// Substitutes TinyXML* Attribute(const char* attributeName) method in order to support reading strings better
// replaces: const char* breakSoundPath = pElem->Attribute("BreakSound");
// with:	 const char* breakSoundPath = readStringAttribute(pElem, "BreakSound");
// returns empty string "" if nothing found.
const char*
Game::readStringAttribute(TiXmlElement* pElem, const char* elementName)
{
	const char* finalResult = "";
	const char* readResult = pElem->Attribute(elementName);
	if (readResult != 0) {
		return readResult;
	}
	return finalResult;
}
//-------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------