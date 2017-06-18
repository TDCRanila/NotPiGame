#include "../Headers/Bullet.h"

#include "../Headers/Enemy.h"
#include "../Headers/Geometry.h"
#include "../Headers/ModelMatrix.h"
#include "../Headers/Player.h"

Bullet::Bullet() {
	// Empty
}

Bullet::~Bullet() {
	// Empty
}

int Bullet::getDamage() {
	return this->bulletDamage;	
}
void Bullet::setDamage(int d) {
	this->bulletDamage = d;	
}

RepeaterPlasma::RepeaterPlasma(glm::vec3 pos, glm::vec3 speed, int bulletDamage, float bulletLifeTime, glm::vec3 bulletDirection) {
	this->setPos(pos);
	this->setAcceleration(speed);
	this->setDamage(bulletDamage);
	this->bulletDirection = bulletDirection;
	this->maxLifeTime = bulletLifeTime;
}

RepeaterPlasma::~RepeaterPlasma() {
	// Empty
}

void RepeaterPlasma::update(float deltaTime) {
	GameObject::update();	
	glm::vec3 speed = this->getAcceleration() * deltaTime * this->bulletDirection;
	this->setPos(this->getPos() + speed);
	this->lifeTime += deltaTime;
	this->getModel()->setRotations(glm::vec3(lifeTime * 25));
	
	if (this->lifeTime > maxLifeTime) {
		this->setDeadBool(true);
		this->getModel()->deleteModel = true;
		return;
	}
	
	// Create a ray - player/Camera front and player pos
	Ray ray(this->getPos(), this->bulletDirection, true);
	
	int closestEnemyID;
	float closestLength		= 999999999;
	bool firstHit			= true;
	bool hit = false;
	
	// Check for enemy hit
	for (int i = 0; i < enemyVectorPointer->size(); i++) {
		Enemy* currentEnemy = (*enemyVectorPointer)[i];
		
		// If the enemy is dead then goto the next enemy in the vector
		if (currentEnemy->getDeadBool() || currentEnemy->dying == true || currentEnemy->canDie == true) { continue; }
		
		// Check Ray-AABB collision
		float length = -1;
		float maxLength = this->getAcceleration().x * deltaTime;
		if (ray.rayBoxIntersect(currentEnemy->hitBox, length)) {
			if (length > 0 && maxLength > length) {
				// There is a hit in front of the bullet
				hit = true;
				if (firstHit || (length < closestLength)) {
					firstHit = false;
					closestEnemyID = i;
					closestLength = length;
					currentEnemy->AddHit();
				}
			}
		}
	}
	if (hit == true) {
		this->setDeadBool(true);
		this->getModel()->deleteModel = true;
		this->getModel()->draw = false;
		(*enemyVectorPointer)[closestEnemyID]->setHitBool(true);
		(*enemyVectorPointer)[closestEnemyID]->setBulletHit(this);
	}
}

TurretPlasma::TurretPlasma(glm::vec3 pos, glm::vec3 speed, int bulletDamage, float bulletLifeTime, glm::vec3 bulletDirection) {
	this->setPos(pos);
	this->setAcceleration(speed);
	this->setDamage(bulletDamage);
	this->bulletDirection = bulletDirection;
	this->maxLifeTime = bulletLifeTime;
}

TurretPlasma::~TurretPlasma() {
	// Empty
}

void TurretPlasma::update(float deltaTime) {
	GameObject::update();	
	glm::vec3 speed = this->getAcceleration() * deltaTime * this->bulletDirection;
	this->setPos(this->getPos() + speed);
	this->lifeTime += deltaTime;
	this->getModel()->setRotations(glm::vec3(lifeTime * 25));
	
	if (this->lifeTime > maxLifeTime) {
		this->setDeadBool(true);
		this->getModel()->deleteModel = true;
		return;
	}
	
	// Create a ray - enemy direction
	Ray ray(this->getPos(), this->bulletDirection, true);
	bool hit = false;
			
		// If the enemy is dead then goto the next enemy in the vector
	if (!playerPointer->getDeadBool()) {
		
		// Check Ray-AABB collision
		float length = -1;
		float maxLength = this->getAcceleration().x * deltaTime;
		if (ray.rayBoxIntersect(playerPointer->hitBox, length)) {
			if (length > 0 && maxLength > length) {
				// There is a hit in front of the bullet
				hit = true;
				playerPointer->AddHit();
			}
		}
	}
	if (hit == true) {
		this->setDeadBool(true);
		this->getModel()->deleteModel = true;
		this->getModel()->draw = false;
		playerPointer->setHitBool(true);
		playerPointer->setBulletHit(this);
	}
}	