#include "../Headers/Level.h"

#include "../Headers/Camera.h"
#include "../Headers/ResourceManager.h"
////#include "../Headers/Input.h"

////#include "../Headers/ModelMatrix.h"
////#include "../Headers/ShaderClass.h"
#include "../Headers/MD2Model.h"
#include "../Headers/OBJModel.h"

Level::Level(std::vector<ModelMatrix*>* gameModels, std::vector<ModelMatrix*>* terrainModels, 
			 std::vector<ModelMatrix*>* backGroundModels, std::vector<GUI*>* guiSystems,
			 Camera* camera,	Input* input, ResourceManager* rm) 
			: gameObjectsModels(gameModels), terrainObjectsModels(terrainModels), backgroundObjectsModels(backGroundModels), GUISystems(guiSystems),
	         cameraPointer(camera), inputPointer(input), resourceManager(rm) 
{
	CreateLevel();
}

Level::~Level() {
	DeleteLevel();
}

void Level::setExitBool(bool exit)	{ this->exitLevel = exit; }
bool Level::getExitBool()	{ return this->exitLevel; }

void Level::setResetBool(bool reset) { this->resetLevel = reset; }
bool Level::getResetBool() { return this->resetLevel; }
	
void Level::setPauseBool(bool pause) {this->pauseLevel = pause; }
bool Level::getPauseBool()	{ return this->pauseLevel; }

//////////////////////////////////////////////
// Initialised the gameplay systems
//////////////////////////////////////////////
void Level::CreateLevel() {
	// Loading all the resourecs
	LoadResources();
		
	// Creating our objects
	initLevelSystem();
	spawnPlayer();
	enemyManager->storePlayerPointer(player);
	enemyManager->storeGameModels(gameObjectsModels);
	enemyManager->storeRMPointer(resourceManager);
	spawnPickups();
	
	// Create our GUI
	initGameGUI();
}

//////////////////////////////////////////////
// Updating the game each frame
//////////////////////////////////////////////
void Level::UpdateLevel(float deltaTime) {
	// If this button is pressed, exit the level
	//if (inputPointer->getKey(KEY_F4)) { this->setExitBool(true); return; }
	//
	//// If this button is pressed, exit the level
	//if (inputPointer->getKey(KEY_F5)) { this->setResetBool(true); return; }
	//
	//// If this button is pressed, pause the level
	//if (inputPointer->getKey(KEY_P)) { this->setPauseBool(true); }
	
		// Update the resource manager - to remove models that are no longer needed
	this->resourceManager->updateResources();
	
	// Update the camera
	this->cameraPointer->cameraUpdate(deltaTime);
	
	// Update the player
	this->player->update(deltaTime);
	
	// Update the enemies
	enemyManager->updateManager(deltaTime);
	
	// Update all the pickups
	pickupManager->updatePickups(deltaTime);
}

//////////////////////////////////////////////
// Delete everything that the Game owns
//////////////////////////////////////////////
void Level::DeleteLevel() {
	// Now delete all the game objects
	delete player;
	delete enemyManager;
	delete grid;
		
	// Delete all the models
	terrainObjectsModels->clear();
	backgroundObjectsModels->clear();
	gameObjectsModels->clear();
	// Remove the Game GUI System
	GUISystems->pop_back();
	
	// Delete all the resources
	resourceManager->deleteResources();
}

//////////////////////////////////////////////
// All the game systems
//////////////////////////////////////////////
void Level::initLevelSystem() {
	// Create LevelObject class
	ModelMatrix* terrain = resourceManager->getOriginalModelOBJ(0);
	terrain->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	terrain->setScale(glm::vec3(1.0f, 1.0f, 1.0f));
	this->terrainObjectsModels->push_back(terrain);
		
	// Create the skybox
	CubeMap* skybox		= resourceManager->getCubeMap(0);
	skybox->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	this->backgroundObjectsModels->push_back(skybox);
	
		// Create a 2D grid for the level.
	glm::vec2 startgridPos	= glm::vec2(-10.0f - (20.f * 8), -10.0f + (20.f * 3));
	glm::vec2 cellDimension = glm::vec2(20.f, 20.f);
	int rowAmount			= 18;
	int cellsAmount			= 11;
	this->grid = new Grid2D(startgridPos, cellDimension, rowAmount, cellsAmount, false, true);
	
//		for (int i = 0; i < grid->getGrid().size(); i++) {
//		for (int j = 0; j < grid->getGrid()[i].size(); j++) {
//			glm::vec2 cellPos = grid->getGrid()[i][j]->getPosition();			
//		
//			ModelMatrix* cube = new OBJModel();
//			cube = resourceManager->getModelOBJ(1);
//			cube->setPosition(glm::vec3(cellPos.x, 5.0f, cellPos.y));
//		
//			this->gameObjectsModels->push_back(cube);
//	} }
	
	// Create the enemy manager
	enemyManager = new EnemyManager();
	
	// Initialise other systems - ai, keys, collectables, enemies
	spawnEnemies();	
}

void Level::initGameGUI() {
	// Allocate the GUI
	gameGUI = new GUI();
	
	// Creating the static components
	GUIComponent crosshair(crosshairFile, guiShader);
	crosshair.setPosition(glm::vec3(0.0f, 0.0f, -0.255f));
	crosshair.setScale(glm::vec3(0.035f, 0.035f, 1.0f));
	gameGUI->components.push_back(crosshair);
	
	GUISystems->push_back(gameGUI);
}

void Level::spawnPlayer() {
	// Values for the player
	glm::vec2 playerCell(2, 8);
	glm::vec2 cellPos(grid->getCell2D(playerCell.x, playerCell.y)->getPosition());
	glm::vec3 startPos(cellPos.x, 4.0f, cellPos.y);
	glm::vec3 dimension(2.0f, 6.0f, 2.0f);
	glm::vec3 speed(25.f, 25.f, 25.f);
	int health = 100;
	
	player = new Player(startPos, speed, health, dimension, resourceManager);
	
	// Store different pointers that we will need inside of the player object
	player->storeInput(inputPointer);
	player->storeCamera(cameraPointer);
	player->storeTerrainObjects(terrainObjectsModels);
	player->storeEnemyVector(enemyManager->getEnemyVector());
	
	// Give the player the guns
	player->createGuns(gameObjectsModels, resourceManager);	
}

void Level::spawnEnemies() {
	// Creating the 2D Grid positions for Loader Bots
	std::vector<glm::vec2> lp;
	lp.push_back(glm::vec2(0, 1));
	lp.push_back(glm::vec2(2, 1));
	lp.push_back(glm::vec2(3, 4));
	lp.push_back(glm::vec2(4, 4));
	lp.push_back(glm::vec2(4, 9));
	lp.push_back(glm::vec2(4, 10));
	lp.push_back(glm::vec2(5, 4));
	lp.push_back(glm::vec2(5, 9));
	lp.push_back(glm::vec2(5, 10));
	lp.push_back(glm::vec2(9, 7));
	lp.push_back(glm::vec2(9, 9));
	lp.push_back(glm::vec2(11, 7));
	lp.push_back(glm::vec2(11, 8));
	lp.push_back(glm::vec2(11, 9));
	lp.push_back(glm::vec2(12, 7));
	lp.push_back(glm::vec2(12, 8));
	lp.push_back(glm::vec2(12, 9));
	
	// Creating the positions for the Turret Bots
	std::vector<glm::vec2> tp;
	tp.push_back(glm::vec2(3, 8));
	tp.push_back(glm::vec2(1, 1));
	tp.push_back(glm::vec2(7, 1));
	tp.push_back(glm::vec2(10, 6));
	tp.push_back(glm::vec2(10, 10));
	tp.push_back(glm::vec2(13, 6));
	tp.push_back(glm::vec2(13, 10));
	
	for (int i = 0; i < lp.size(); i++) {
		// Values for the loader bot
		glm::vec2 cp = grid->getCell2D(lp[i].x, lp[i].y)->getPosition();
		glm::vec3 startPos(glm::vec3(cp.x, 5.0f, cp.y));
		glm::vec3 dimension(2.0f, 4.0f, 2.0f);
		glm::vec3 speed(25.f, 25.f, 25.f);
		
		Enemy* enemy = new LoaderBot(startPos, speed, 20, dimension, resourceManager);
		enemy->storeModel(resourceManager->getModelMD2(6));
		enemy->storePlayerPointer(player);
		enemy->storeTerrainObjects(terrainObjectsModels);
		enemy->getModel()->setScale(glm::vec3(0.175f, 0.175f, 0.175f));
		
		enemyManager->addEnemy(enemy);
		this->gameObjectsModels->push_back(enemy->getModel());
	}
	for (int i = 0; i < tp.size(); i++) {
		// Values for the turret bot
		glm::vec2 cp = grid->getCell2D(tp[i].x, tp[i].y)->getPosition();
		glm::vec3 startPos(glm::vec3(cp.x, 3.0f, cp.y));
		glm::vec3 dimension(2.5f, 4.0f, 2.5f);
		glm::vec3 speed(25.f, 25.f, 25.f);
		
		Enemy* enemy = new TurretBot(startPos, speed, 50, dimension, resourceManager);
		enemy->storeModel(resourceManager->getModelMD2(7));
		enemy->storePlayerPointer(player);
		enemy->storeTerrainObjects(terrainObjectsModels);
		enemy->getModel()->setScale(glm::vec3(0.175f, 0.175f, 0.175f));
		
		enemyManager->addEnemy(enemy);
		this->gameObjectsModels->push_back(enemy->getModel());
	}
}

void Level::spawnPickups() {
	pickupManager = new PickupManager();
	
	// Creating all the healthpacks for the level
	std::vector<glm::vec2> ph;
	ph.push_back(glm::vec2(1, 7));
	ph.push_back(glm::vec2(2, 7));
	ph.push_back(glm::vec2(0, 0));
	ph.push_back(glm::vec2(2, 0));
	
	for (int i = 0; i < ph.size(); i++) {
		glm::vec2 cp = grid->getCell2D(ph[i].x, ph[i].y)->getPosition();
		glm::vec3 startPos(glm::vec3(cp.x, 3.0f, cp.y));
		glm::vec3 dimension(3.0f, 3.0f, 3.0f);
		
		Pickup* h = new HealthPack(startPos, dimension, 25, player);
		h->storeModel(resourceManager->getModelOBJ(3));
		
		pickupManager->addPickup(h);
		this->gameObjectsModels->push_back(h->getModel());
	}
	
	// Creating all the ammo packs
	std::vector<glm::vec2> pa;
	pa.push_back(glm::vec2(7, 7));
	pa.push_back(glm::vec2(4, 8));
	pa.push_back(glm::vec2(3, 6));
	pa.push_back(glm::vec2(1, 0));
	
	for (int i = 0; i < pa.size(); i++) {
		glm::vec2 cp = grid->getCell2D(pa[i].x, pa[i].y)->getPosition();
		glm::vec3 startPos(glm::vec3(cp.x, 4.0f, cp.y));
		glm::vec3 dimension(3.0f, 3.0f, 3.0f);
		Pickup* a = new AmmoPack(startPos, dimension, - 1, 25, player);
		a->storeModel(resourceManager->getModelOBJ(4));
		
		pickupManager->addPickup(a);
		this->gameObjectsModels->push_back(a->getModel());
	}
	
	/* Create finish block */ {
		glm::vec2 fp(17, 8);
		glm::vec2 cp = grid->getCell2D(fp.x, fp.y)->getPosition();
		glm::vec3 startPos(glm::vec3(cp.x, 4.0f, cp.y));
		glm::vec3 dimension(3.0f, 3.0f, 3.0f);
		
		Pickup* f = new FinishBlock(startPos, dimension, player);
		f->storeModel(resourceManager->getModelOBJ(5));
		
		pickupManager->addPickup(f);
		this->gameObjectsModels->push_back(f->getModel());
	}	
	
}

//////////////////////////////////////////////
// Loads all the resource we need for the level
//////////////////////////////////////////////
void Level::LoadResources() {
	//* The order of loading matters when storing in the pointers/IDs in the resource manager  *//
	
#pragma region Shaders
	std::string mainShaderVert = "../Resources/Shaders/mShader.vert";
	std::string mainShaderFrag = "../Resources/Shaders/mShader.frag";
	this->mainShader	= this->resourceManager->createShader(mainShaderVert, mainShaderFrag);		// #0
	
	std::string md2ShaderVert = "../Resources/Shaders/MD2Shader.vert";
	std::string md2ShaderFrag = "../Resources/Shaders/MD2Shader.frag";
	this->md2Shader		= this->resourceManager->createShader(md2ShaderVert, md2ShaderFrag);		// #1
	
	std::string skyboxShaderVert = "../Resources/Shaders/skyboxShader.vert";
	std::string skyboxShaderFrag = "../Resources/Shaders/skyboxShader.frag";
	this->skyboxShader	= this->resourceManager->createShader(skyboxShaderVert, skyboxShaderFrag);	// #2
	
	std::string GUIshaderVert = "../Resources/Shaders/GUIShader.vert";
	std::string GUIshaderFrag = "../Resources/Shaders/GUIShader.frag";
	this->guiShader		= this->resourceManager->createShader(GUIshaderVert, GUIshaderFrag);	// #3
	
#pragma endregion Here we load in all the shaders that we need in the game

#pragma region OBJ Models
	/* Model - #0 - Terrain OBJ */ {
		std::string modelFile	= "../Resources/Models/TerrainModel/level00.obj";
		this->resourceManager->loadAndStoreOBJ(modelFile, this->mainShader);
	}
	/* Model - #1 - Repeater Plasma Prism OBJ */ {
		std::string modelFile	= "../Resources/Models/Weapons/Plasma/plasma.obj";
		this->resourceManager->loadAndStoreOBJ(modelFile, this->mainShader);
	}
	/* Model - #2 - Turret Bot Plasma Prism OBJ */ {
		std::string modelFile	= "../Resources/Models/Characters/TurretDroid/plasma.obj";
		this->resourceManager->loadAndStoreOBJ(modelFile, this->mainShader);
	}
	
	/* Model - #3 - HealthPack OBJ */ {
		std::string modelFile	= "../Resources/Models/Pickups/Healthpack/healthbox.obj";
		this->resourceManager->loadAndStoreOBJ(modelFile, this->mainShader);
}
	/* Model - #4 - Ammo Pack  OBJ */ {
		std::string modelFile	= "../Resources/Models/Pickups/Ammopack/ammobox.obj";
		this->resourceManager->loadAndStoreOBJ(modelFile, this->mainShader);
}
	/* Model - #5 - Finish Block OBJ */ {
		std::string modelFile	= "../Resources/Models/Pickups/Finishbox/finishbox.obj";
		this->resourceManager->loadAndStoreOBJ(modelFile, this->mainShader);
}
#pragma endregion Here we load in all the OBJmodels that we need in the game
	
#pragma region MD2 Models
		/* Model - #0 - Player Pistol Model */ {
		std::string modelFile		= "../Resources/Models/Weapons/Pistol/view.md2";
		std::string textureFile		= "../Resources/Models/Weapons/Pistol/view.png";
		this->resourceManager->loadAndStoreMD2(modelFile, textureFile, this->md2Shader);
	}
	
		/* Model - #1 - Player Pistol Flash Model */ {
		std::string modelFile		= "../Resources/Models/Weapons/Pistol/flash.md2";
		std::string textureFile		= "../Resources/Models/Weapons/Pistol/flash.png";
		this->resourceManager->loadAndStoreMD2(modelFile, textureFile, this->md2Shader);
	}
	
		/* Model - #2 - Player Shotgun Model */ {
		std::string modelFile		= "../Resources/Models/Weapons/Shotgun/view.md2";
		std::string textureFile		= "../Resources/Models/Weapons/Shotgun/view.png";
		this->resourceManager->loadAndStoreMD2(modelFile, textureFile, this->md2Shader);
	}
	
		/* Model - #3 - Player Shotgun Flash Model */ {
		std::string modelFile		= "../Resources/Models/Weapons/Shotgun/flash.md2";
		std::string textureFile		= "../Resources/Models/Weapons/Shotgun/flash2.png";
		this->resourceManager->loadAndStoreMD2(modelFile, textureFile, this->md2Shader);
	}
	
		/* Model - #4 - Player Plasma Rifle Model */ {
		std::string modelFile		= "../Resources/Models/Weapons/Plasma/view.md2";
		std::string textureFile		= "../Resources/Models/Weapons/Plasma/view.png";
		this->resourceManager->loadAndStoreMD2(modelFile, textureFile, this->md2Shader);
	}
	
		/* Model - #5 - Player Plasma Rifle Flash Model */ {
		std::string modelFile		= "../Resources/Models/Weapons/Plasma/flash.md2";
		std::string textureFile		= "../Resources/Models/Weapons/Plasma/flash.png";
		this->resourceManager->loadAndStoreMD2(modelFile, textureFile, this->md2Shader);
	}
		/* Model - #6 - LoaderBot Model */ {
		std::string modelFile		= "../Resources/Models/Characters/BattleDroid/BattleDroid.md2";
		std::string textureFile		= "../Resources/Models/Characters/BattleDroid/BattleDroid.png";
		this->resourceManager->loadAndStoreMD2(modelFile, textureFile, this->md2Shader);
	}
		/* Model - #7 - TurretBot Model */ {
		std::string modelFile		= "../Resources/Models/Characters/TurretDroid/TurretDroid.md2";
		std::string textureFile		= "../Resources/Models/Characters/TurretDroid/TurretDroid.png";
		this->resourceManager->loadAndStoreMD2(modelFile, textureFile, this->md2Shader);
	}
#pragma endregion
	
#pragma region Textures

#pragma endregion Here we load in all the textures that we need in the game

#pragma region CubeMaps // Skybox needs to be last in the render call
	/* CubeMap - #0 - Space Skybox */ {
		const char* right	= "../Resources/Textures/Skybox/redSpace_right.png";
		const char* left	= "../Resources/Textures/Skybox/redSpace_left.png";
		const char* top		= "../Resources/Textures/Skybox/redSpace_top.png";
		const char* bottom	= "../Resources/Textures/Skybox/redSpace_bottom.png";
		const char* front	= "../Resources/Textures/Skybox/redSpace_back.png";
		const char* back	= "../Resources/Textures/Skybox/redSpace_front.png";
		CubeMap* skybox = this->resourceManager->createCubeMap(right, left, top, bottom, back, front, this->skyboxShader);
	} // Give it its on scope - so it doesn't intervene with other variables
#pragma endregion Here we load in the cubemaps that we need in the game

}