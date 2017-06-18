#include "../Headers/Player.h"

#include "../Headers/ResourceManager.h"

#include "../Headers/Input.h"
#include "../Headers/ModelMatrix.h"
#include "../Headers/MD2Model.h"
#include "../Headers/Camera.h"
#include "../Headers/OBJModel.h"

#include "../Headers/Gun.h"
#include "../Headers/AmmoPack.h"
#include "../Headers/HealthPack.h"
#include "../Headers/FinishBlock.h"

////#include <GLES2/gl2.h>

Player::Player() {
	// Empty
}

Player::Player(glm::vec3 startPos, glm::vec3 speed, int health, glm::vec3 dimension, ResourceManager* RM) {	
	this->m_pos			= startPos;
	this->m_accel		= speed;
	this->m_health		= health;
	this->m_dimension	= dimension;
	this->setNoModel(true);
	
	this->standDimension	= dimension.y;
	this->crouchDimension	= dimension.y / 2;
	this->standAccel		= speed;
	this->crounchAccel		= glm::vec3(speed.x / 2.f, speed.y / 2.f, speed.z / 2.f);
	
	// Create the hitbox of the enemy
	Box temp(this->m_pos, this->m_dimension.x, this->m_dimension.y, this->m_dimension.z);
	this->hitBox = temp;
}

Player::~Player() {
	// Make sure to delete all the guns
	for (int i = 0; i < this->m_guns.size(); i++) {
		delete this->m_guns[i];
	}
}

void Player::update(float deltaTime) {
	// Update the player directional vectors
	updateVectors();
	
	// Passive update for gravity
	passiveUpdate(deltaTime);
	
	// Check for collision
	checkCollision();
	
	// Check hit
	checkHit();
	
	// Check pickup
	checkPickup();
	
	// Check for player input - Handles all the events when a button is pressed
	checkPlayerInput(deltaTime);
}

void Player::storePickUpPointer(Pickup* pickup) {
	this->currentPickupPointer = pickup;	
}

void Player::createGuns(std::vector<ModelMatrix*>* gameModels, ResourceManager* RM) {
#pragma region Pistol
	// Adding all the weapons of the player
	Gun* pistol = new Pistol(pistolName, pistolDamage, pistolFireRate, pistolDefaultAmmo, pistolAnimSpeed, pistolPenetration);
	
	// Setting up the model
	pistol->storeGameModelVector(gameModels);
	pistol->storeGunModels(RM->getOriginalModelMD2(0), RM->getOriginalModelMD2(1)); // Pistol - Gun And Flash Models
	pistol->storeResourceManager(RM);
	pistol->getModel(0)->setPosition(glm::vec3(-0.15f, 0.025f,  0.075f));
	pistol->getModel(1)->setPosition(glm::vec3(-0.025f, 0.035f, 0.0175f));
	pistolPos			= pistol->getModel(0)->getPositon();
	pistolFlashPos		= pistol->getModel(1)->getPositon();
	pistol->getModel(0)->setRotationsDegrees(glm::vec3(0.0f, 90.f, 0.0f));
	pistol->getModel(1)->setRotationsDegrees(glm::vec3(0.0f, 90.f, 0.0f));
	pistol->getModel(0)->setScale(glm::vec3(0.0325f, 0.0325f, 0.0325f));
	pistol->getModel(1)->setScale(glm::vec3(0.0325f, 0.0325f, 0.0325f));
	pistol->getModel(0)->cancelView = true;
	pistol->getModel(1)->cancelView = true;
	pistol->m_weaponModels[0]->draw = true;
	pistol->m_weaponModels[1]->draw = true;
	
	pistol->storeEnemyVector(this->enemyVectorPointer);
	m_guns.push_back(pistol);
	gameModels->push_back(pistol->getModel(0));
	gameModels->push_back(pistol->getModel(1));
	
#pragma endregion
#pragma region Repeater
	// Creating the Repeater Gun
	Gun* repeater = new Repeater(repeaterName, repeaterDamage, repeaterFireRate, repeaterProjectileSpeed, repeaterDefaultAmmo, repeaterAnimSpeed, repeaterPenetration);
	repeater->storeGameModelVector(gameModels);
	repeater->storeResourceManager(RM);
	repeater->storeGunModels(RM->getOriginalModelMD2(4), RM->getOriginalModelMD2(5)); // Repeater - Gun And Flash Models
	repeater->getModel(0)->setPosition(glm::vec3(-0.35f, 0.0f, -0.125f));
	repeater->getModel(1)->setPosition(glm::vec3(-0.35f, 0.0f, -0.125f));
	repeaterPos			= repeater->getModel(0)->getPositon();
	repeaterFlashPos	= repeater->getModel(1)->getPositon();
	repeater->getModel(0)->setRotationsDegrees(glm::vec3(0.0f, 90.f, 0.0f));
	repeater->getModel(1)->setRotationsDegrees(glm::vec3(0.0f, 90.f, 0.0f));
	repeater->getModel(0)->setScale(glm::vec3(0.0325f, 0.0325f, 0.0325f));
	repeater->getModel(1)->setScale(glm::vec3(0.0325f, 0.0325f, 0.0325f));
	repeater->getModel(0)->cancelView = true;
	repeater->getModel(1)->cancelView = true;
	repeater->m_weaponModels[0]->draw = false;
	repeater->m_weaponModels[1]->draw = false;
	
	repeater->storeEnemyVector(this->enemyVectorPointer);
	m_guns.push_back(repeater);
	gameModels->push_back(repeater->getModel(0));
	gameModels->push_back(repeater->getModel(1));
#pragma endregion
#pragma region Shotgun
	// Creating the Shotgun
	Gun* shotgun = new Shotgun(shotgunName, shotgunDamage, shotgunFireRate, shotgunPelletAmount, shotgunDefaultAmmo, shotgunAnimSpeed, shotgunPenetration);
	shotgun->storeGameModelVector(gameModels);
	shotgun->storeResourceManager(RM);
	shotgun->storeGunModels(RM->getOriginalModelMD2(2), RM->getOriginalModelMD2(3)); // Shotgun - Gun and Flash Models
	shotgun->getModel(0)->setPosition(glm::vec3(-0.45f, -0.05f, 0.235f));
	shotgun->getModel(1)->setPosition(glm::vec3(-0.45f, -0.05f, 0.235f));
	shotgunPos		= shotgun->getModel(0)->getPositon();
	shotgunFlashPos = shotgun->getModel(1)->getPositon();
	shotgun->getModel(0)->setRotationsDegrees(glm::vec3(0.0f, 90.f, 0.0f));
	shotgun->getModel(1)->setRotationsDegrees(glm::vec3(0.0f, 90.f, 0.0f));
	shotgun->getModel(0)->setScale(glm::vec3(0.0275f, 0.0325f, 0.0325f));
	shotgun->getModel(1)->setScale(glm::vec3(0.0275f, 0.0325f, 0.0325f));
	shotgun->getModel(0)->cancelView = true;
	shotgun->getModel(1)->cancelView = true;
	shotgun->m_weaponModels[0]->draw = false;
	shotgun->m_weaponModels[1]->draw = false;
	
	shotgun->storeEnemyVector(this->enemyVectorPointer);
	m_guns.push_back(shotgun);
	gameModels->push_back(shotgun->getModel(0));
	gameModels->push_back(shotgun->getModel(1));
#pragma endregion
}

void Player::passiveUpdate(float deltaTime) {
	// Put the camera ontop of the player
	if (freeCamera == false) {
		this->m_camera->Position = this->m_pos;
	}
	
	// Update the player its position due to gravity
	this->m_pos += GRAVITY * -this->m_up;
	
	// Update the hitbox
	hitBox.updateBoxPos(this->m_pos);
	
	// Always call this function for every gun, to reset them
	for (int i = 0; i < this->m_guns.size(); i++) {
		this->m_guns[i]->storePlayerPosition(this->m_pos);
		this->m_guns[i]->storePlayerFront(this->m_cameraFront);
		this->m_guns[i]->resetGun(deltaTime);
	}
	
	// Update the Repeater bullets
	Repeater* repeater = static_cast<Repeater*>(this->m_guns[1]);
	if (!repeater->repeaterBullets.empty()) {
		for (int i = 0; i < repeater->repeaterBullets.size(); i++) {
			repeater->repeaterBullets[i]->update(deltaTime);
			if (repeater->repeaterBullets[i]->getDeadBool()) {
				repeater->repeaterBullets[i]->getModel()->draw = false;
				delete repeater->repeaterBullets[i];
				repeater->repeaterBullets[i] = repeater->repeaterBullets.back();
				repeater->repeaterBullets.pop_back();
	} } }
	
	// Switch the gun model to the correct angle
	if (switchGunSide == true) {
		if (gunRight) {
			m_guns[0]->m_weaponModels[0]->setScale(glm::vec3(-0.0325f, 0.0325f, 0.0325f));
			m_guns[0]->m_weaponModels[1]->setScale(glm::vec3(-0.0325f, 0.0325f, 0.0325f));
			m_guns[1]->m_weaponModels[0]->setScale(glm::vec3(-0.0325f, 0.0325f, 0.0325f));
			m_guns[1]->m_weaponModels[1]->setScale(glm::vec3(-0.0325f, 0.0325f, 0.0325f));
			m_guns[2]->m_weaponModels[0]->setScale(glm::vec3(-0.0275f, 0.0325f, 0.0325f));
			m_guns[2]->m_weaponModels[1]->setScale(glm::vec3(-0.0275f, 0.0325f, 0.0325f));
			m_guns[0]->m_weaponModels[0]->setPosition(glm::vec3(-pistolPos.x	, pistolPos.y			, pistolPos.z));
			m_guns[0]->m_weaponModels[1]->setPosition(glm::vec3(-pistolFlashPos.x	, pistolFlashPos.y	, pistolFlashPos.z));
			m_guns[1]->m_weaponModels[0]->setPosition(glm::vec3(-repeaterPos.x		, repeaterPos.y		, repeaterPos.z));
			m_guns[1]->m_weaponModels[1]->setPosition(glm::vec3(-repeaterFlashPos.x	, repeaterFlashPos.y, repeaterFlashPos.z));
			m_guns[2]->m_weaponModels[0]->setPosition(glm::vec3(-shotgunPos.x		, shotgunPos.y		, shotgunPos.z));
			m_guns[2]->m_weaponModels[1]->setPosition(glm::vec3(-shotgunFlashPos.x	, shotgunFlashPos.y	, shotgunFlashPos.z));
			switchGunSide = false;
		} else {
			m_guns[0]->m_weaponModels[0]->setScale(glm::vec3( 0.0325f, 0.0325f, 0.0325f));
			m_guns[0]->m_weaponModels[1]->setScale(glm::vec3( 0.0325f, 0.0325f, 0.0325f));
			m_guns[1]->m_weaponModels[0]->setScale(glm::vec3( 0.0325f, 0.0325f, 0.0325f));
			m_guns[1]->m_weaponModels[1]->setScale(glm::vec3( 0.0325f, 0.0325f, 0.0325f));
			m_guns[2]->m_weaponModels[0]->setScale(glm::vec3( 0.0275f, 0.0325f, 0.0325f));
			m_guns[2]->m_weaponModels[1]->setScale(glm::vec3( 0.0275f, 0.0325f, 0.0325f));
			m_guns[0]->m_weaponModels[0]->setPosition(glm::vec3(pistolPos.x			, pistolPos.y		, pistolPos.z));
			m_guns[0]->m_weaponModels[1]->setPosition(glm::vec3(pistolFlashPos.x	, pistolFlashPos.y	, pistolFlashPos.z));
			m_guns[1]->m_weaponModels[0]->setPosition(glm::vec3(repeaterPos.x		, repeaterPos.y		, repeaterPos.z));
			m_guns[1]->m_weaponModels[1]->setPosition(glm::vec3(repeaterFlashPos.x	, repeaterFlashPos.y, repeaterFlashPos.z));
			m_guns[2]->m_weaponModels[0]->setPosition(glm::vec3(shotgunPos.x		, shotgunPos.y		, shotgunPos.z));
			m_guns[2]->m_weaponModels[1]->setPosition(glm::vec3(shotgunFlashPos.x	, shotgunFlashPos.y	, shotgunFlashPos.z));
			switchGunSide = false;
		}
	}
}

void Player::checkPlayerInput(float deltaTime) {
	//// Input - Player Movement
	//if (this->m_input->getKey(KEY_W)) {
	//	this->PlayerMovementEnum = PLAYER_FORWARD;
	//	doMovement(deltaTime);
	//}
	//
	//if (this->m_input->getKey(KEY_S)) {
	//	this->PlayerMovementEnum = PLAYER_BACKWARD;
	//	doMovement(deltaTime);
	//}
	//
	//if (this->m_input->getKey(KEY_A)) {
	//	this->PlayerMovementEnum = PLAYER_LEFT;
	//	doMovement(deltaTime);
	//}
	//
	//if (this->m_input->getKey(KEY_D)) {
	//	this->PlayerMovementEnum = PLAYER_RIGHT;
	//	doMovement(deltaTime);
	//}
	//
	//if (this->m_input->getKey(KEY_LEFTCTRL)) {
	//	this->PlayerMovementEnum = PLAYER_CROUCH;
	//	doMovement(deltaTime);
	//} else { this->m_dimension.y = standDimension; this->m_accel = standAccel; } // To reset the dimension
	//
	//	// Input - Player Switch Weapons - 0 = RailGun Pistol
	//if (this->m_input->getKey(KEY_1)) {
	//	if (currentWeaponSelect != 0 && this->m_guns[currentWeaponSelect]->canSwitch == true) {
	//		switchWeapon(0);
	//	}
	//}
	//
	//	// Input - Player Switch Weapons - 1 = Plasma Reapeter
	//if (this->m_input->getKey(KEY_2)) {
	//	if (currentWeaponSelect != 1 && this->m_guns[currentWeaponSelect]->canSwitch == true) {
	//		switchWeapon(1);
	//	}
	//}
	//
	//	// Input - Player Switch Weapons - 2 = Hellshot Shotgun
	//if (this->m_input->getKey(KEY_3)) {
	//	if (currentWeaponSelect != 2 && this->m_guns[currentWeaponSelect]->canSwitch == true) {
	//		switchWeapon(2);
	//	}
	//}
	//
	//// Input - Player Shooting Actions
	//if ((this->m_input->TheMouse.LeftButton == true) || (this->m_input->getKey(KEY_SPACE))) {
	//	weaponShoot(deltaTime);
	//}
	//
	//// Input - Player Switch Gun Angle
	//if (this->m_input->getKey(KEY_V)) {
	//	if (pressedSwitch == false) {
	//		if (gunRight == false) { gunRight = true; } 
	//		else { gunRight = false; }
	//		switchGunSide = true;
	//		pressedSwitch = true;
	//	}
	//} if (!this->m_input->getKey(KEY_V)) { pressedSwitch = false; }
	//	
	//// Reset Button
	//if (this->m_input->getKey(KEY_F12)) {
	//	this->m_pos = glm::vec3(0.0f, 10.0f, 0.0f);
	//}
	
}

void Player::doMovement(float deltaTime) {
	if (this->PlayerMovementEnum == PLAYER_FORWARD) {
		float accel  = this->m_accel.z * deltaTime;
		this->m_pos += accel * this->m_movementFront;
	}
	
	if (this->PlayerMovementEnum == PLAYER_BACKWARD) {
		float accel  = this->m_accel.z * deltaTime;
		this->m_pos -= accel * this->m_movementFront;
	}
	
	if (this->PlayerMovementEnum == PLAYER_LEFT) {
		float accel = this->m_accel.x * deltaTime;
		this->m_pos -= accel * this->m_cameraRight;
	}
	
	if (this->PlayerMovementEnum == PLAYER_RIGHT) {
		float accel = this->m_accel.x * deltaTime;
		this->m_pos += accel * this->m_cameraRight;
	}
	
	if (this->PlayerMovementEnum == PLAYER_CROUCH) {
		this->m_dimension.y = crouchDimension;
		this->m_accel		= crounchAccel;
	}
	
}

void Player::weaponShoot(float deltaTime) {
	// Check which weapon we have active and goto its function
	if (this->m_guns[currentWeaponSelect]->getAmmo() != 0) { // If the gun ins't empty then shoot
		if (this->m_guns[currentWeaponSelect]->canShoot == true) {
			this->m_guns[currentWeaponSelect]->shootEvent(deltaTime);
			this->m_guns[currentWeaponSelect]->canShoot = false;
			this->m_guns[currentWeaponSelect]->canSwitch = false;
	} }
}

void Player::switchWeapon(int weaponNum) {
	this->currentWeaponSelect = weaponNum;
	printf("SWITCH GUN TO - %s\n", this->m_guns[weaponNum]->m_weaponName.c_str());
	this->storeModel(this->m_guns[weaponNum]->m_weaponModels[0]);	
	// Disable all the guns and then switch back to the current gun
	this->m_guns[0]->m_weaponModels[0]->draw = false;
	this->m_guns[0]->m_weaponModels[1]->draw = false;
	this->m_guns[1]->m_weaponModels[0]->draw = false;
	this->m_guns[1]->m_weaponModels[1]->draw = false;
	this->m_guns[2]->m_weaponModels[0]->draw = false;
	this->m_guns[2]->m_weaponModels[1]->draw = false;
	
	this->m_guns[weaponNum]->m_weaponModels[0]->draw = true;
	this->m_guns[weaponNum]->m_weaponModels[1]->draw = true;
}

void Player::checkCollision() {
	float objectHeightFromMiddle	= this->m_dimension.y / 2;
	float objectWidthFromMiddle		= this->m_dimension.x / 2;
	// Looping through all the triangles in the world
	for (int k = 0; k < this->terrainPointer->size(); k++) {
		OBJModel* t = static_cast<OBJModel*>((*this->terrainPointer)[k]);
		for (int j = 0; j < t->drawObjects.size(); j++) {
			for (int i = 0; i < t->drawObjects[j].triangles.size(); i++) {
				float length;
						
				{ // Check for a ground collision - Downwards Ray
					glm::vec3 rayDir(0.0f, -1.0f, 0.0f);
					Ray ray(glm::vec3(this->m_pos.x, this->m_pos.y - (objectHeightFromMiddle / 2), this->m_pos.z), rayDir);
					if (ray.rayTriangleIntersect(t->drawObjects[j].triangles[i], length)) {
						if (length < objectHeightFromMiddle) {
							this->m_pos = glm::vec3(this->m_pos.x, this->m_pos.y + (objectHeightFromMiddle - length), this->m_pos.z);
							break;
				} } }
				{ // Check for Right collision - Right Ray
					glm::vec3 rayDir(this->m_right);
					Ray ray(glm::vec3(this->m_pos.x + (objectWidthFromMiddle), this->m_pos.y, this->m_pos.z), rayDir);
					if (ray.rayTriangleIntersect(t->drawObjects[j].triangles[i], length)) {
					//printf("Right Ray Length: %.2f\n", length);
						if (length < objectWidthFromMiddle) {
							this->m_pos = glm::vec3(this->m_pos.x - (objectWidthFromMiddle - length), this->m_pos.y, this->m_pos.z);
							//break;
				} } }
				{ // Check for Left collision - Left Ray
					glm::vec3 rayDir(-this->m_right);
					Ray ray(glm::vec3(this->m_pos.x - (objectWidthFromMiddle), this->m_pos.y, this->m_pos.z), rayDir);
					if (ray.rayTriangleIntersect(t->drawObjects[j].triangles[i], length)) {
						//printf("Left Ray Length: %.2f\n", length);
						if (length < objectWidthFromMiddle) {
							this->m_pos = glm::vec3(this->m_pos.x + (objectWidthFromMiddle - length), this->m_pos.y, this->m_pos.z);
							//break;
				} } }
				{ // Check for Front Collision - Forward Ray
					glm::vec3 rayDir(this->m_front);
					Ray ray(glm::vec3(this->m_pos.x, this->m_pos.y, this->m_pos.z + objectWidthFromMiddle), rayDir);
					if (ray.rayTriangleIntersect(t->drawObjects[j].triangles[i], length)) {
						//printf("Front Ray Length: %.2f\n", length);
						if (length < objectWidthFromMiddle) {
							this->m_pos = glm::vec3(this->m_pos.x, this->m_pos.y, this->m_pos.z - (objectWidthFromMiddle - length));
							//break;
				} } }
				{ // Check for Back Collision - Backward Ray
					glm::vec3 rayDir(-this->m_front);
					Ray ray(glm::vec3(this->m_pos.x, this->m_pos.y, this->m_pos.z - objectWidthFromMiddle), rayDir);
					if (ray.rayTriangleIntersect(t->drawObjects[j].triangles[i], length)) {
						//printf("Back Ray Length: %.2f\n", length);
						if (length < objectWidthFromMiddle) {
							this->m_pos = glm::vec3(this->m_pos.x, this->m_pos.y, this->m_pos.z + (objectWidthFromMiddle - length));
							//break;
				} } }
				
	} } } // End of Loop
}

void Player::checkHit() {
	if (this->m_hit) {
	// Remove health per hit
		for (int i = 0; i < hits; i++) {
			// When hit reduce the health of the enemy with the damage of w/e gun it got hit by
			int currentHealth = this->getHealth();
			if (this->currentGunHit != nullptr) {
				this->setHealth(currentHealth - this->currentGunHit->getDamage());
			}
			if (this->currentBulletHit != nullptr) {
				this->setHealth(currentHealth - this->currentBulletHit->getDamage());
			}
		}
		this->currentGunHit		= nullptr;
		this->currentBulletHit	= nullptr;
		hits = 0;
		
		// There is no more hit
		this->m_hit = false;
		
		// If the player has no more health - this results in a 
		if (this->getHealth() <= 0) {
			this->setDeadBool(true);
			// Game Over
		}
	}
}

void Player::checkPickup() {
	if ((this->pickUpBool) && (currentPickupPointer != nullptr)) {
		if (currentPickupPointer->identifier == -1) { return; }
		
		// If healthpack
		if (currentPickupPointer->identifier == 1) {
			HealthPack* temp = static_cast<HealthPack*>(currentPickupPointer);
			this->setHealth(this->getHealth() + temp->m_healthValue);
			
			// Reset the boolean
			pickUpBool = false;
			currentPickupPointer = nullptr;
			return;
		}
		if (currentPickupPointer->identifier == 2) {
			// AmmoPack stuff
			AmmoPack* temp = static_cast<AmmoPack*>(currentPickupPointer);
			this->m_guns[0]->setAmmo(this->m_guns[0]->getAmmo() + pistolDefaultAmmo);
			this->m_guns[1]->setAmmo(this->m_guns[1]->getAmmo() + repeaterDefaultAmmo);	
			this->m_guns[2]->setAmmo(this->m_guns[2]->getAmmo() + shotgunDefaultAmmo);	
			
			// Reset the boolean
			pickUpBool = false;
			currentPickupPointer = nullptr;
			return;
		}
		if (currentPickupPointer->identifier == 0) {
			// Finish Block stuff
			finishedLevel = true;
			
			// Reset the boolean
			pickUpBool = false;
			currentPickupPointer = nullptr;
			return;
		}

		pickUpBool = false;
	}
}

void Player::updateVectors() {
	this->m_cameraFront	= this->m_camera->Front;
	this->m_cameraRight	= this->m_camera->Right;
	this->m_cameraUp	= this->m_camera->Up;
	
	// Movement vector - y value gets ignored - this is for the direction of the movement of the player
	this->m_movementFront = glm::cross(-this->m_camera->Right, this->m_up);
}