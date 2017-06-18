#pragma once

#include "../Headers/Graphics.h"
#include "../Headers/Camera.h"
#include "../Headers/StateMachine.h"

//#include "../Headers/ResourceManager.h"
//#include "../Headers/GUI.h"

#include <vector>

class Level; class GUI; class ModelMatrix; class ResourceManager;

class MainGame {
public:
	MainGame(Window* window, int screenWidth, int screenHeight);
	~MainGame();
		
	void UpdateMainGame(float deltaTime);
	void Draw();
	
	void setGameExit(bool exit);
	bool getGameExit();
	
protected:
	// Menu Functions
	void InitMenu();
	void UpdateMenu();
	
	// Pause Menu Functions
	void InitPauseMenu();
	void UpdatePauseMenu();
		
	// Main Control over the level
	void InitLevel();
	void UpdateLevel();
	void ResetLevel();
	void DeleteLevel();
					
	// These is used by the state machine
	static void InitMenuThunk(void* instance);
	static void UpdateMenuThunk(void* instance);
	
	static void InitPauseThunk(void* instance);
	static void UpdatePauseThunk(void* instance);
		
	static void InitLevelThunk(void* instance);
	static void UpdateLevelThunk(void* instance);
	static void ResetLevelThunk(void* instance);
	static void DeleteLevelThunk(void* instance);
		
	State* InitMenuState	= nullptr;
	State* UpdateMenuState	= nullptr;
	
	State* InitPauseState	= nullptr;
	State* UpdatePauseState = nullptr;
		
	State* InitLevelState	= nullptr;
	State* UpdateLevelState	= nullptr;
	State* ResetLevelState	= nullptr;
	State* DeleteLevelState = nullptr;
		
private:
	// Frame Step controller
	int MAX_PHYSICS_STEPS	= 1;
	float MAX_DELTA_TIME	= 0.125f;
	float FRAME_STEP_FPS	= 0.0f;
	
	// Core components for our Game
    Window*             windowHandle    = nullptr;
	StateMachine*		gameController	= nullptr;
	ResourceManager*	resourceManager	= nullptr;	
	Input*				input			= nullptr;
	
	// Pointer to our current level
	Level*				currentLevel	= nullptr;
	
	// Useful variables
	bool  m_exitGame		= false;
	float m_deltaTime		= 0.0f;
		
	// Camera + Default Camera Settings
	Camera* m_mainCamera	= nullptr;
	float m_aspect		= (16.0f / 9.0f);
    float m_nearZ			= 0.25f;
    float m_farZ			= 300.f;
					
	// Draw Objects Vector Pointers - All The Models
	std::vector<GUI*>* m_GUISystems							= nullptr;
	std::vector<ModelMatrix*>* m_gameObjectsModels			= nullptr;
	std::vector<ModelMatrix*>* m_terrainObjectsModels		= nullptr;
	std::vector<ModelMatrix*>* m_backgroundObjectsModels	= nullptr;	
};