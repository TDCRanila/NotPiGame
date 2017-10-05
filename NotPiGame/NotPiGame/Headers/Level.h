#pragma once

#include "../Headers/Grid.h"
#include "../Headers/GUI.h"

#include "../Headers/GameObject.h"
#include "../Headers/Player.h"
#include "../Headers/EnemyManager.h"
#include "../Headers/LoaderBot.h"
#include "../Headers/TurretBot.h"

#include "../Headers/Pickup.h"
#include "../Headers/HealthPack.h"
#include "../Headers/AmmoPack.h"
#include "../Headers/FinishBlock.h"

#include <vector>

class ResourceManager; class Camera; class Input; class Shader; // FW Declaration

class Level {
public:
	Level(std::vector<ModelMatrix*>* gameModels, std::vector<ModelMatrix*>* terrainModels, 
		  std::vector<ModelMatrix*>* backGroundModels, std::vector<GUI*>* guiSystems,
		  Camera* camera, Input* input, ResourceManager* rm);
	~Level();
	
	void CreateLevel();
	void UpdateLevel(float deltaTime);
	void DeleteLevel();
	void LoadResources();
	
	void setExitBool(bool exit);
	bool getExitBool();
	void setResetBool(bool reset);
	bool getResetBool();
	void setPauseBool(bool pause);
	bool getPauseBool();
			
protected:
	// *** Game Specific objects/pointers/functions and variables ***//
	void initLevelSystem();
	void initGameGUI();
	void spawnPlayer();
	void spawnEnemies();
	void spawnPickups();
	
	Player* player					= nullptr;
	EnemyManager* enemyManager		= nullptr;
	PickupManager* pickupManager	= nullptr;
	Grid2D*	grid					= nullptr;
	
	// GUI Stuff
	GUI*		gameGUI				= nullptr;
	std::string crosshairFile		= "Resources/Textures/GUI/crosshair.png";
	
	// *** Pointers that we need in order to make the level functonal ***//
	Camera*				cameraPointer	= nullptr;
	Input*				inputPointer	= nullptr;
	ResourceManager*	resourceManager	= nullptr;	
	
	bool exitLevel	= false;
	bool resetLevel = false;
	bool pauseLevel = false;
	
	std::vector<GUI*>*		   GUISystems				= nullptr;  // GUI Components
	std::vector<ModelMatrix*>* gameObjectsModels		= nullptr;	// Enemy, Player, Bullets
	std::vector<ModelMatrix*>* terrainObjectsModels		= nullptr;	// Terrain/Level, static objects
	std::vector<ModelMatrix*>* backgroundObjectsModels	= nullptr;	// CubeMaps
	
	// Shader Pointers
	Shader* mainShader		= nullptr;
	Shader* md2Shader		= nullptr;
	Shader* skyboxShader	= nullptr;
	Shader* guiShader		= nullptr;
};