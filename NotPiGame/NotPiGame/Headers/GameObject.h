#pragma once

#include "../glm/include/common.hpp"

#include <vector>

const float GRAVITY		= 0.5f;
const float FRICTION	= 0.0f; 

class ModelMatrix; class Input; class Camera; class Enemy; class Bullet; class Player; class EnemyManager; class Gun; class Box; // FW Declaration

class GameObject {
public:
	GameObject();
	~GameObject();
	
	// Basic update for the gameobject - also places the modelmatrix on top of the GameObjects its position	
	void update();
	
	void storeModel(ModelMatrix* model); // Stores pointer of the MD2s, OBJs, ...
	void storeInput(Input* input); // Stores the pointer to the input class in memory
	void storeCamera(Camera* camera); // Stores a pointer to the camera class that is in memory
	void storeTerrainObjects(std::vector<ModelMatrix*>* terrain);
	void storeEnemyVector(std::vector<Enemy*>* enemies);
	
	ModelMatrix*	getModel();
	Input*			getInput();
	Camera*			getCamera();
				
	glm::vec3	getPos();
	void		setPos(glm::vec3 pos);
	
	glm::vec3	getDimension();
	void		setDimension(glm::vec3 dim);
	
	glm::vec3	getAcceleration();
	void		setAcceleration(glm::vec3 accel);
	
	glm::vec3	getVelocity();
	void		setVelocity(glm::vec3 vel);
	
	glm::vec3	getMaxVelocity();
	void		setMaxVelocity(glm::vec3 maxVel);
	
	glm::vec3	getRotation();
	void		setRotation(glm::vec3 rot);
	
	int			getHealth();
	void		setHealth(int health);
		
	bool		getHitBool();
	void		setHitBool(bool hit);
	
	void		setGunHit(Gun* g);
	void		setBulletHit(Bullet* b);
	void		AddHit();
	
	bool		getDeadBool();
	void		setDeadBool(bool dead);
	
	bool		getRemoveModelBool();
	void		setRemoveModelBool(bool noModel);
	
	void		setNoModel(bool b); // Boolean to set at the start if a gameobject doesn't want a model
	
	// Some pointers that a game object can hold
	ModelMatrix* m_model	= nullptr;
	Input*		 m_input	= nullptr;
	Camera*		 m_camera	= nullptr;
	std::vector<ModelMatrix*>*	terrainPointer		= nullptr;
	std::vector<Enemy*>*		enemyVectorPointer	= nullptr;
	Player*	playerPointer	 = nullptr;
	EnemyManager* EMPointer	 = nullptr;
	Gun*	currentGunHit	 = nullptr;
	Bullet* currentBulletHit = nullptr;
	int hits = 0;
		
protected:
	// Game Values
	int		m_health;
	bool	m_hit		= false;
	bool	m_isDead	= false;
	bool	m_noModel	= false;
	bool	m_removeModel = false;	
		
	glm::vec3 m_pos; // Game Object Position not the model
	glm::vec3 m_rotation; // Basically the rotation where the game object is facing. IN DEGREES
	
	glm::vec3 m_front = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 m_up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 m_right = glm::vec3(1.0f, 0.0f, 0.0f);
	
	glm::vec3 m_dimension; // Width, Height, Depth
	
	glm::vec3 m_accel; // Amount of acceleration per tick - vector because different speed for moving forward, sideways and up
	glm::vec3 m_velocity; // Total velocity of the object
	glm::vec3 m_maxVelocity;
};