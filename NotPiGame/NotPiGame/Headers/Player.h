#pragma once

#include "../Headers/Geometry.h"
#include "../Headers/Pickup.h"
#include "../Headers/GameObject.h"

#include <string>

class Gun; class ResourceManager; class Enemy; // FW Declaration

enum Player_Movement {
	PLAYER_FORWARD,
	PLAYER_BACKWARD,
	PLAYER_LEFT,
	PLAYER_RIGHT,
	PLAYER_JUMP,
	PLAYER_CROUCH
};

class Player : public GameObject {
public:
	Player();
	Player(glm::vec3 startPos, glm::vec3 speed, int health, glm::vec3 dimension, ResourceManager* RM);
	~Player();
		
	void storePickUpPointer(Pickup* pickup);
	
	void createGuns(std::vector<ModelMatrix*>* gameModels, ResourceManager* RM);
	
	void update(float deltaTime);
	void passiveUpdate(float deltaTime);
	void updateVectors();
	
	void checkPlayerInput(float deltaTime); // Check for input specific to the player and "redirects" the input to the corensponding event/function
	void checkCollision();
	void checkHit();
	void checkPickup();
	
	void doMovement(float deltaTime);
	void weaponShoot(float deltaTime); // Or put this in a weapon class
	void switchWeapon(int weaponNum);
	
	bool freeCamera		= false; // Locks the camera to the player if false
	bool pickUpBool		= false;
	bool finishedLevel	= false;
	Box hitBox;
	
private:
	Player_Movement PlayerMovementEnum;
	Pickup*	currentPickupPointer		= nullptr;
	
	// Player Weapon Inventory
	std::vector<Gun*> m_guns;
	int currentWeaponSelect = 0; // Default to 0
	
	// Pistol Stats
	std::string pistolName  = "Railgun Pistol";
	int pistolDamage		= 3;
	float pistolFireRate	= 0.325f;
	int pistolDefaultAmmo	= 32;
	float pistolAnimSpeed	= 20.f;
	bool pistolPenetration	= true;
	glm::vec3 pistolPos;
	glm::vec3 pistolFlashPos;
			
	// Repeater Stats
	std::string repeaterName = "Plasma Repeater";
	float repeaterProjectileSpeed	= 125.f;
	int repeaterDamage				= 3;
	float repeaterFireRate			= 0.100f;
	int repeaterDefaultAmmo			= 120;
	float repeaterAnimSpeed			= 17.5f;
	bool repeaterPenetration		= false;
	glm::vec3 repeaterPos;
	glm::vec3 repeaterFlashPos;
	
	// Shotgun Stats
	std::string shotgunName = "Railgun shotgun";
	int shotgunPelletAmount	= 15;
	int shotgunDamage		= 2;
	float shotgunFireRate	= 0.85f;
	int shotgunDefaultAmmo	= 18;
	float shotgunAnimSpeed	= 14.5f;
	bool shotgunPenetration	= false;
	glm::vec3 shotgunPos;
	glm::vec3 shotgunFlashPos;
	
	
	// Booleans to control whether we want the gun on the left or the right
	bool switchGunSide	= false;
	bool gunRight		= false;
	bool pressedSwitch	= false;
	
	// Values that we store
	float standDimension;
	float crouchDimension;
	glm::vec3 standAccel;
	glm::vec3 crounchAccel;
	
	// Vectors from the camera class
	glm::vec3 m_cameraFront;
	glm::vec3 m_cameraRight;
	glm::vec3 m_cameraUp;
	glm::vec3 m_movementFront;
};