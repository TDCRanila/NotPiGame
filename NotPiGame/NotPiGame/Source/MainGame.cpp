#include "../Headers/MainGame.h"

#include "../Headers/ModelMatrix.h"

#include "../glm/include/common.hpp"
#include "../glm/include/gtc/matrix_transform.hpp"

#include <algorithm>

MainGame::MainGame(Window* window, int screenWidth, int screenHeight) {
    windowHandle                = window;
	gameController              = new StateMachine();
	resourceManager				= new ResourceManager();
	input                       = new Input();
	
	m_GUISystems				= new std::vector<GUI*>;
	m_terrainObjectsModels		= new std::vector<ModelMatrix*>;
	m_backgroundObjectsModels	= new std::vector<ModelMatrix*>;
	m_gameObjectsModels			= new std::vector<ModelMatrix*>;
	
	// Creating the camera of the game
	m_mainCamera = new Camera();
	m_mainCamera->storeInput(input);
	
	// Store the screen sizes
	this->m_screenWidth     = screenWidth;
	this->m_screenHeight    = screenHeight;
	
	// Now create all the states and push the first state
	gameController->createNewState(InitMenuState      , "Init Menu State"     , &InitMenuThunk);
	gameController->createNewState(UpdateMenuState    , "Update Menu State"   , &UpdateMenuThunk);
	
	gameController->createNewState(InitPauseState     , "Init Game Pause State"  , &InitPauseThunk);
	gameController->createNewState(UpdatePauseState   , "Update Game Pause State", &UpdatePauseThunk);
	
	gameController->createNewState(InitLevelState     , "Init Level State"    , &InitLevelThunk);
	gameController->createNewState(UpdateLevelState   , "Update Level State"  , &UpdateLevelThunk);
	gameController->createNewState(ResetLevelState	  , "Reset Level State"	  , &ResetLevelThunk);
	gameController->createNewState(DeleteLevelState   , "Delete Level State"  , &DeleteLevelThunk);
	
	// Set the gameController to initialise the game or the menu
	gameController->pushState(InitLevelState);
}
MainGame::~MainGame() {	
	// Now the core systems
    windowHandle = nullptr;
	delete resourceManager;
	resourceManager = nullptr;
	delete gameController;
	gameController    = nullptr;
	delete m_mainCamera;
	m_mainCamera = nullptr;
	delete input;
	input = nullptr;	
		
	// Now delete the draw vectors - objects within should already have been destroyed - only the pointers are left
	delete m_terrainObjectsModels;
	delete m_backgroundObjectsModels;
	delete m_gameObjectsModels;
}

void MainGame::setGameExit(bool exit) { this->m_exitGame = exit; }
bool MainGame::getGameExit()          { return this->m_exitGame; }

void MainGame::UpdateMainGame(float deltaTime) {
	// First store the deltaTime inside of the MainGame object
	
	// Screenshot
	//if (input->getKey(KEY_F10)) { screenshotter.Screenshot(m_screenWidth, m_screenHeight, 0, 0);}
	
	// FrameStep Controller
	int timeStepCount = 0;
	while (deltaTime > 0.0f && timeStepCount < MAX_PHYSICS_STEPS) {
		FRAME_STEP_FPS = std::min(deltaTime, MAX_DELTA_TIME);
		
		// Set the deltaTime
		m_deltaTime = FRAME_STEP_FPS;
		
		// Here we update the MainGame with whatever function is now active in the state machine
		gameController->update(this);
	
		timeStepCount++;
	}
}
void MainGame::Draw() {
	// Store the matrices so we don't have to calculate late them every time
	glm::mat4 viewMatrix		= m_mainCamera->GetViewMatrix();
	glm::mat4 projectionMatrix	= m_mainCamera->GetPerspectiveProjectionMatrix(m_aspect, m_nearZ, m_farZ);
	// Draw all the GUI stuff
	for (int j = 0; j < m_GUISystems->size(); j++) {
		for (int i = 0; i < m_GUISystems->size(); i++) {	
			(*m_GUISystems)[j]->components[i].Update(m_deltaTime);
			(*m_GUISystems)[j]->components[i].Draw(viewMatrix, projectionMatrix);
	} }
	// Draw all the Game objects
	for (int i = 0; i < m_gameObjectsModels->size(); i++) {
		if ((*m_gameObjectsModels)[i]->deleteModel == true) {
			(*m_gameObjectsModels)[i] = (*m_gameObjectsModels).back();
			(*m_gameObjectsModels).pop_back();
			continue;	
		}
			(*m_gameObjectsModels)[i]->Update(m_deltaTime);
			(*m_gameObjectsModels)[i]->Draw(viewMatrix, projectionMatrix);
	}
	// Draw all the Terrain Objects
	for (int i = 0; i < m_terrainObjectsModels->size(); i++) {
		(*m_terrainObjectsModels)[i]->Update(m_deltaTime);
		(*m_terrainObjectsModels)[i]->Draw(viewMatrix, projectionMatrix);
	}
	// Draw all the Background Objects
	for (int i = 0; i < m_backgroundObjectsModels->size(); i++) {	
		(*m_backgroundObjectsModels)[i]->Update(m_deltaTime);
		(*m_backgroundObjectsModels)[i]->Draw(viewMatrix, projectionMatrix);
	}
}

// Menu Functions
void MainGame::InitMenu() {}
void MainGame::UpdateMenu() {}

// Pause Menu Functions
void MainGame::InitPauseMenu()    {
	gameController->popState(); // No Longer init pause menu state
	gameController->pushState(UpdatePauseState); // Now update the pause menu
}
void MainGame::UpdatePauseMenu() {
	// Check when to exit the pause menu
	//if (input->getKey(KEY_LEFTBRACE)) {
	//	currentLevel->setPauseBool(false);
	//	gameController->popState(); // No Longer pause the level
	//}
}

// Main Control over the level
void MainGame::InitLevel() {
	// Create a new level
	currentLevel = new Level(m_gameObjectsModels, m_terrainObjectsModels, m_backgroundObjectsModels, m_GUISystems, m_mainCamera, input, resourceManager);
	
	// No longer init the level
	gameController->popState();
	
	// Now update the level
	gameController->pushState(UpdateLevelState);
}
void MainGame::UpdateLevel() {
	if (currentLevel->getExitBool()) {
		gameController->popState(); // No Longer Update
		gameController->pushState(DeleteLevelState);
	}
	
	if (currentLevel->getResetBool()) {
		gameController->popState(); // No Longer Update
		gameController->pushState(ResetLevelState);
	}
	
	if (currentLevel->getPauseBool()) {
		gameController->pushState(InitPauseState);
    }
	
	// Update the level
	currentLevel->UpdateLevel(m_deltaTime);
}
void MainGame::ResetLevel() {
	// Delete the current level and make a new one
	delete currentLevel;
	currentLevel = new Level(m_gameObjectsModels, m_terrainObjectsModels, m_backgroundObjectsModels, m_GUISystems, m_mainCamera, input, resourceManager);
	
	gameController->popState(); // No longer init the reset level state
	gameController->pushState(UpdateLevelState); // And now update the level again
}
void MainGame::DeleteLevel() {
	// Delete the level object
	delete currentLevel;
	
	// No longer delete the level
	gameController->popState();
	
	// Now got to the main menu
	//gameController->pushState(UpdateMenuState);
	this->setGameExit(true);
}

void MainGame::InitMenuThunk(void* instance)    { static_cast<MainGame*>(instance)->InitMenu();         }
void MainGame::UpdateMenuThunk(void* instance)  { static_cast<MainGame*>(instance)->UpdateMenu();       }
void MainGame::InitPauseThunk(void* instance)   { static_cast<MainGame*>(instance)->InitPauseMenu();    }
void MainGame::UpdatePauseThunk(void* instance) { static_cast<MainGame*>(instance)->UpdatePauseMenu();  }
void MainGame::InitLevelThunk(void* instance)   { static_cast<MainGame*>(instance)->InitLevel();        }
void MainGame::UpdateLevelThunk(void* instance) { static_cast<MainGame*>(instance)->UpdateLevel();      }
void MainGame::ResetLevelThunk(void* instance)	{ static_cast<MainGame*>(instance)->ResetLevel();		}
void MainGame::DeleteLevelThunk(void* instance) { static_cast<MainGame*>(instance)->DeleteLevel();      }