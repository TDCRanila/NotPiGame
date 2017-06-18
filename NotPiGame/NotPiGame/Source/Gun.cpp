#include "../Headers/Gun.h"

#include "../Headers/ModelMatrix.h"
#include "../Headers/MD2Model.h"
#include "../Headers/Geometry.h"
#include "../Headers/Utilities.h"
#include "../Headers/ResourceManager.h"

#include "../Headers/Enemy.h"

Gun::Gun() {
	// Empty
}

Gun::~Gun() {
	// Empty
}

void Gun::storeGunModels(ModelMatrix* pointerGun, ModelMatrix* pointerFlash) {
	MD2Model* gunModel		= static_cast<MD2Model*>(pointerGun);
	MD2Model* flashModel	= static_cast<MD2Model*>(pointerFlash);
	
	this->m_weaponModels[0] = gunModel;
	this->m_weaponModels[1] = flashModel;
}

ModelMatrix* Gun::getModel(int i) {
	return (this->m_weaponModels[i]);
}

void Gun::storeEnemyVector(std::vector<Enemy*>* enemies) {
	this->m_enemyVectorPointer = enemies;
}

void Gun::storePlayerPosition(glm::vec3 playerPos) {
	this->m_playerPosition = playerPos;
}

void Gun::storeGameModelVector(std::vector<ModelMatrix*>* gameModelVec) {
	this->m_gameModelVec = gameModelVec;
}

void Gun::storePlayerFront(glm::vec3 frontVector) {
	this->m_playerFront = frontVector;
}

void Gun::storeResourceManager(ResourceManager* RM) {
	this->m_resourceManager = RM;
}

void Gun::shootEvent(float deltaTime) {
	// Check if the gun has any ammo
	if (this->canShoot == true) {
		// Shoot, Animate and Remove Ammo
		shootGun(deltaTime);
		animate();
		this->m_ammo--;
		
		// printf("Ammo - %d\n", this->m_ammo);
	}
}

void Gun::resetGun(float deltaTime) {
	if (this->canShoot == false) {
		if (m_gunTimer >= m_fireRate) {
			this->canShoot  = true;
			this->canSwitch = true;
			// printf("GUN READY\n");
			m_gunTimer = 0;
		} else {
			m_gunTimer += deltaTime;
			// printf("Gun Timer - %.2f\n", m_gunTimer);
	} }
	
	if (this->m_weaponModels[0]->getAnimateFinnished()) {
		// Reset the animation for the weapon
		this->m_weaponModels[0]->setFrameState(0);
		this->m_weaponModels[0]->setAnimSpeed(this->m_animSpeed);
		this->m_weaponModels[0]->setAnimate(false);
		// And for the flash
		this->m_weaponModels[1]->setFrameState(0);
		this->m_weaponModels[1]->setAnimSpeed(this->m_animSpeed);
		this->m_weaponModels[1]->setAnimate(false);
	}
}

void Gun::animate() {
	// Set the right animation for the weapon
	this->m_weaponModels[0]->setFrameState(3);
	this->m_weaponModels[0]->setAnimSpeed(this->m_animSpeed);
	this->m_weaponModels[0]->setAnimate(true);
	// And for the flash
	this->m_weaponModels[1]->setFrameState(3);
	this->m_weaponModels[1]->setAnimSpeed(this->m_animSpeed);
	this->m_weaponModels[1]->setAnimate(true);
}

int Gun::getAmmo() {
	return this->m_ammo;
}

void Gun::setAmmo(int ammo) {
	this->m_ammo = ammo;
}

int Gun::getDamage() {
	return this->m_damage;	
}
void Gun::setDamage(int d) {
	this->m_damage = d;	
}

#pragma region Pistol

Pistol::Pistol(std::string name, int damage, float fireRate, int defaultAmmo, float animSpeed, bool penetration) {
	this->m_weaponName = name;
	this->m_damage = damage;
	this->m_fireRate = fireRate;
	this->m_ammo = defaultAmmo;
	this->m_animSpeed = animSpeed;
	this->m_penetration = penetration;
}

Pistol::~Pistol() {
	this->m_weaponModels[0] = nullptr;
	this->m_weaponModels[1] = nullptr;
}

void Pistol::shootGun(float deltaTime) {	
	// Create a ray at the direction of the playe
	Ray ray(this->m_playerPosition, this->m_playerFront, true);
	
	int closestEnemyID;
	float closestLength		= 999999999;
	bool firstHit			= true;
	bool hit = false;
	
	// Loop through all the enemies
	for (int i = 0; i < m_enemyVectorPointer->size(); i++) {
		Enemy* currentEnemy = (*m_enemyVectorPointer)[i];
		
		// If the enemy is dead then goto the next enemy in the vector
		if (currentEnemy->getDeadBool() || currentEnemy->dying == true || currentEnemy->canDie == true) { continue; }
		
		// Check Ray-AABB collision
		float length = -1;
		if (ray.rayBoxIntersect(currentEnemy->hitBox, length)) {
			if (length > 0) {
				// There is a hit in front of us!
				hit = true;
				if (!m_penetration) {
					if (firstHit || (length < closestLength)) {
						firstHit = false;
						closestEnemyID = i;
						closestLength = length;
						currentEnemy->AddHit();
						break;
					}
				} else {
					currentEnemy->setHitBool(true);
					currentEnemy->setGunHit(this);
					currentEnemy->AddHit();
					continue;
				}	
			}
		}
	}
	if (hit == true && !m_penetration) {
		(*m_enemyVectorPointer)[closestEnemyID]->setHitBool(true);
		(*m_enemyVectorPointer)[closestEnemyID]->setGunHit(this);
	}
}
#pragma endregion

#pragma region Repeater

Repeater::Repeater(std::string name, int damage, float fireRate, float projSpeed, int defaultAmmo, float animSpeed, bool penetration) {
	this->m_weaponName = name;
	this->m_damage = damage;
	this->m_fireRate = fireRate;
	this->m_projectileSpeed = glm::vec3(projSpeed);
	this->m_ammo = defaultAmmo;
	this->m_animSpeed = animSpeed;
	this->m_penetration = penetration;
}

Repeater::~Repeater() {
	this->m_weaponModels[0] = nullptr;
	this->m_weaponModels[1] = nullptr;
}

void Repeater::shootGun(float deltaTime) {
	// Create a bullet
	glm::vec3 p(this->m_playerPosition);
	glm::vec3 f(this->m_playerFront);
	glm::vec3 offset = f * 2.75f;
	RepeaterPlasma* bullet = new RepeaterPlasma(glm::vec3(p + offset), glm::vec3(this->m_projectileSpeed), this->m_damage, this->repeaterBulletLifeTime, this->m_playerFront);
	bullet->storeModel((ModelMatrix*)m_resourceManager->getModelOBJ(1));
	bullet->storeEnemyVector(m_enemyVectorPointer);
	repeaterBullets.push_back(bullet);
	m_gameModelVec->push_back(bullet->getModel());	
}

#pragma endregion

#pragma region Shotgun

Shotgun::Shotgun(std::string name, int damage, float fireRate, int pelletAmount, int defaultAmmo, float animSpeed, bool penetration) {
	this->m_weaponName = name;
	this->m_damage = damage;
	this->m_fireRate = fireRate;
	this->m_pelletsPerShot = pelletAmount;
	this->m_ammo = defaultAmmo;
	this->m_animSpeed = animSpeed;
	this->m_penetration = penetration;
}

Shotgun::~Shotgun() {
	this->m_weaponModels[0] = nullptr;
	this->m_weaponModels[1] = nullptr;
}

void Shotgun::shootGun(float deltaTime) {
	// Create a ray - player/Camera front and player pos
	for (int shots = 0; shots < m_pelletsPerShot; shots++) {
		// Calculate in a random range direction for the the ray
		float xRandom = Utilities::randomFloatNumRange(-0.145f, 0.145f);
		float yRandom = Utilities::randomFloatNumRange(-0.145f, 0.145f);
		glm::vec3 rayDir = glm::vec3(this->m_playerFront.x + xRandom, this->m_playerFront.y + yRandom, this->m_playerFront.z);
		glm::vec3 randomDir = glm::normalize(rayDir);
		Ray ray(this->m_playerPosition, randomDir, true);
	
		int closestEnemyID;
		float closestLength		= 999999999;
		bool firstHit			= true;
		bool hit = false;
	
		// Loop through all the enemies
		for (int i = 0; i < m_enemyVectorPointer->size(); i++) {
			Enemy* currentEnemy = (*m_enemyVectorPointer)[i];
		
			// If the enemy ís dead then goto the next enemy in the vector
			if (currentEnemy->getDeadBool() || currentEnemy->dying == true || currentEnemy->canDie == true) { continue; }
		
			// Check Ray-AABB collision
			float length = -1;
			if (ray.rayBoxIntersect(currentEnemy->hitBox, length)) {
				if (length > 0) {
					// There is a hit in front of us!
					hit = true;
					if (!m_penetration) {
						if (firstHit || (length < closestLength)) {
							firstHit = false;
							closestEnemyID = i;
							closestLength = length;
							currentEnemy->AddHit();
						}
					} else {
						currentEnemy->setHitBool(true);
						currentEnemy->setGunHit(this);
						currentEnemy->AddHit();
						continue;
					}	
				}
			}
		}
		
		if (hit == true && !m_penetration) {
			(*m_enemyVectorPointer)[closestEnemyID]->setHitBool(true);
			(*m_enemyVectorPointer)[closestEnemyID]->setGunHit(this);
		}
	}
}
#pragma endregion