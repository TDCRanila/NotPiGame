#include "../Headers/TurretBot.h"

#include "../Headers/MD2Model.h"
#include "../Headers/Utilities.h"
#include "../Headers/EnemyManager.h"
#include "../Headers/ResourceManager.h"
#include "../Headers/OBJModel.h"

#include "../Headers/Gun.h"

TurretBot::TurretBot(glm::vec3 startPos, glm::vec3 speed, int health, glm::vec3 dimension, ResourceManager* RM) {
	this->m_pos = startPos;
	this->m_accel = speed;
	this->m_health = health;
	this->m_dimension = dimension;
	
	// Create all the states
	stateMachine.createNewState(gotoIdleState		, "Goto Idle State"		, &gotoIdleThunk);
	stateMachine.createNewState(onIdleState			, "Update Idle State"	, &OnIdleThunk);
	stateMachine.createNewState(gotoSearchingState	, "Goto Search State"	, &gotoSearchingThunk);
	stateMachine.createNewState(onSearchingState	, "Update Search State"	, &OnSearchingThunk);
	stateMachine.createNewState(gotoShootingState	, "Goto Shoot State"	, &gotoShootingThunk);
	stateMachine.createNewState(onShootingState		, "Update Shoot State"	, &OnShootingThunk);
	
	// Set default state on creation
	stateMachine.pushState(gotoIdleState);
	
	// Create the hitbox of the enemy
	Box temp(this->m_pos, this->m_dimension.x, this->m_dimension.y, this->m_dimension.z);
	this->hitBox = temp;
}

TurretBot::~TurretBot() {
	// Empty
}

void TurretBot::update(float deltaTime) {
	GameObject::update();
	this->m_deltaTime = deltaTime;
		
	// Update the enemy
	if ((!dying) && (!canDie)) {
		// Update the enemy its variables
		hitBox.updateBoxPos(this->m_pos);
		updateRange();
		
		// Exectute function pointer
		stateMachine.update(this);
		
		// Check for a hit by the player
		checkHit();
		
		// And finally animate it
		animate();
	} else {
		// This updates the dead enemy
		deathUpdate(deltaTime);
		// And finally animate it
		animate();
	}
	
	// Update the turret its bullets
	if (!m_turretBullets.empty()) {
		for (int i = 0; i < m_turretBullets.size(); i++) {
			m_turretBullets[i]->update(deltaTime);
			if (m_turretBullets[i]->getDeadBool()) {
				m_turretBullets[i]->getModel()->draw = false;
				delete m_turretBullets[i];
				m_turretBullets[i] = m_turretBullets.back();
				m_turretBullets.pop_back();
	} } }
}

void TurretBot::updateRange() {
	glm::vec3 ta(playerPointer->getPos());
	glm::vec3 tu(this->getPos());
	
	float distance = Utilities::calculateSquaredDistance(glm::vec3(ta.x, 0.0f, ta.z), glm::vec3(tu.x, 0.0f, tu.z));
	targetDistance = distance;
	
	if (distance < (turretRange*turretRange)) {
		targetInRange = true;
	} else {
		targetInRange = false;
	}
}

void TurretBot::shootBullet() {
	// Calculate the direction between the target and the turret
	glm::vec3 direction = glm::vec3(playerPointer->getPos() - this->getPos());
	direction = glm::normalize(direction);
	
	// Create a bullet	
	TurretPlasma* bullet = new TurretPlasma(this->getPos(), glm::vec3(this->turretProjSpeed), this->turretDamage, turretBulletLifeTime, direction);
	bullet->storeModel((ModelMatrix*)this->EMPointer->m_resourceManagerPointer->getModelOBJ(2));
	bullet->storeEnemyVector(this->EMPointer->m_enemies);
	bullet->playerPointer = this->EMPointer->playerPointer;
	m_turretBullets.push_back(bullet);
	this->EMPointer->m_gameModelPointer->push_back(bullet->getModel());	
}

bool TurretBot::checkInSight() {
	// Calculate the direction between the target and the turret
	glm::vec3 direction = glm::vec3(playerPointer->getPos() - this->getPos());
	direction = glm::normalize(direction);
	Ray ray(this->getPos(), direction, true);
	bool inSight	= false;
	bool firstHit	= false;
	float closestLength = 9999999.f;
	
	for (int k = 0; k < this->terrainPointer->size(); k++) {
		OBJModel* t = static_cast<OBJModel*>((*this->terrainPointer)[k]);
		for (int j = 0; j < t->drawObjects.size(); j++) {
			for (int i = 0; i < t->drawObjects[j].triangles.size(); i++) {
				float length;
				if (ray.rayTriangleIntersect(t->drawObjects[j].triangles[i], length)) {
					if (length > 0) {
						// There is a hit
						if (firstHit || (length < closestLength)) {
							firstHit = false;
							closestLength = length;
						}
					}
				}
			}
		}
		float squaredLength = closestLength * closestLength;
		// Check if the target is in range and in sight
		if (targetDistance <= squaredLength) {
			inSight = true;
			return true;
		}	
	}
	return false;
}

void TurretBot::gotoIdle() { 
	// No longer go to this state but go to the idle update
	stateMachine.popState();
	stateMachine.pushState(onIdleState);
}
void TurretBot::idle() { 
	if (m_goSearch) {
		stateMachine.pushState(gotoSearchingState);
		//m_goSearch = false;
	}
}
void TurretBot::gotoSearch(){ 
	// No longer go to this state but update the searching state
	stateMachine.popState();
	stateMachine.pushState(onSearchingState);
}

void TurretBot::search() {
	// Check if player is in range
	if (targetInRange) {
		// If the player is in range then we go 
		stateMachine.pushState(gotoShootingState);
	}
}
void TurretBot::gotoShoot() { 
	// No longer goto shooting state
	stateMachine.popState();
	stateMachine.pushState(onShootingState);
}
void TurretBot::shoot() {
	// Shoot if there is no cooldown and check if the player is in-sight
	if (turretFireTimer > turretFireRate) {
		if (checkInSight()) {
			shootBullet();
			turretFireTimer = 0;
		}
	} else { turretFireTimer += m_deltaTime; }
		
	// If the player is still in range then keep shooting otherwise stop the shooting
	if (!targetInRange) {
		stateMachine.popState(); // stop shooting
		if (m_goSearch)		{ stateMachine.pushState(gotoSearchingState);	}
		if (!m_goSearch)	{ stateMachine.pushState(gotoIdleState);		}
	}
}
	
void TurretBot::gotoIdleThunk(void* instance)		{ static_cast<TurretBot*>(instance)->gotoIdle();	}
void TurretBot::OnIdleThunk(void* instance)			{ static_cast<TurretBot*>(instance)->idle();		}
void TurretBot::gotoSearchingThunk(void* instance)	{ static_cast<TurretBot*>(instance)->gotoSearch();	}
void TurretBot::OnSearchingThunk(void* instance)	{ static_cast<TurretBot*>(instance)->search();		}
void TurretBot::gotoShootingThunk(void* instance)	{ static_cast<TurretBot*>(instance)->gotoShoot();	}
void TurretBot::OnShootingThunk(void* instance)		{ static_cast<TurretBot*>(instance)->shoot();		}