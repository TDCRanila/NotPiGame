#include "../Headers/GameObject.h"

#include "../Headers/ModelMatrix.h"
#include "../Headers/Input.h"
#include "../Headers/Camera.h"
#include "../Headers/Player.h"

#include "../Headers/EnemyManager.h"
#include "../Headers/Bullet.h"
#include "../Headers/Gun.h"

GameObject::GameObject() {
	// Set default pos to 0
	this->m_pos = glm::vec3(0.0f, 0.0f, 0.0f);
	// Set default velocity to 0
	this->m_velocity = glm::vec3(0.0f, 0.0f, 0.0f);
}

GameObject::~GameObject() {
	// Empty 	
}

void GameObject::update() {
	// If the model of the game object needs to be removed...
	if (m_removeModel == true) {
		this->getModel()->deleteModel = true;
		return;
	}
	
	// If the has been specified to have no model then skip this
	if (!m_noModel) {
		if (this->m_model == nullptr) {
            std::cout << "GAMEOBJECT HAS NO MODEL ATTACHED" << std::endl;
		} else {			
			// Update the model to the gameobject
			this->m_model->setPosition(this->m_pos);
			this->m_model->setRotationsDegrees(this->m_rotation);
		} 
	}
}

void GameObject::storeModel(ModelMatrix* m) {
	this->m_model = m;
}

void GameObject::storeInput(Input* i) {
	this->m_input = i;
}

void GameObject::storeCamera(Camera* c) {
	this->m_camera = c;
}

void GameObject::storeTerrainObjects(std::vector<ModelMatrix*>* terrain) {
	this->terrainPointer = terrain;
}

void GameObject::storeEnemyVector(std::vector<Enemy*>* enemies) {
	this->enemyVectorPointer = enemies;
}
	
glm::vec3 GameObject::getPos() {
	return this->m_pos;
}

void GameObject::setPos(glm::vec3 pos) {
	this->m_pos = pos;
}

glm::vec3 GameObject::getRotation() {
	return this->m_rotation;
}

void GameObject::setRotation(glm::vec3 rotation) {
	this->m_rotation = rotation;
}

glm::vec3 GameObject::getDimension() {
	return this->m_dimension;
}

void GameObject::setDimension(glm::vec3 dim) {
	this->m_dimension = dim;
}
	
glm::vec3 GameObject::getAcceleration() {
	return this->m_accel;
}

void GameObject::setAcceleration(glm::vec3 accel) {
	this->m_accel = accel;
}

glm::vec3 GameObject::getVelocity() {
	return this->m_velocity;
}

void GameObject::setVelocity(glm::vec3 velocity) {
	this->m_velocity = velocity;
}

glm::vec3 GameObject::getMaxVelocity() {
	return this->m_maxVelocity;
}

void GameObject::setMaxVelocity(glm::vec3 maxVel) {
	this->m_maxVelocity = maxVel;
}

ModelMatrix* GameObject::getModel() {
	return this->m_model;
}

Input* GameObject::getInput() {
	return this->m_input;	
}

Camera* GameObject::getCamera() {
	return this->m_camera;	
}

void GameObject::setNoModel(bool b) {
	this->m_noModel	= b;
}

int	GameObject::getHealth() {
	return this->m_health;	
}
void GameObject::setHealth(int health) {
	this->m_health = health;	
}
	
bool GameObject::getHitBool() {
	return this->m_hit;
}

void GameObject::setHitBool(bool b) {
	this->m_hit = b;
}

void GameObject::setGunHit(Gun* g) {
	this->currentGunHit = g;
}
void GameObject::setBulletHit(Bullet* b) {
	this->currentBulletHit = b;
}

void GameObject::AddHit() {
	this->hits++;
}

bool GameObject::getDeadBool() {
	return this->m_isDead;
}

void GameObject::setDeadBool(bool b) {
	this->m_isDead = b;
}

bool GameObject::getRemoveModelBool() {
	return this->m_removeModel;
}
void GameObject::setRemoveModelBool(bool noModel) {
	this->m_removeModel = noModel;
}