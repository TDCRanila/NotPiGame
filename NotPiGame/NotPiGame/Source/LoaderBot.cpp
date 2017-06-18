#include "../Headers/LoaderBot.h"

#include "../Headers/Gun.h"
#include "../Headers/Bullet.h"
#include "../Headers/MD2Model.h"

LoaderBot::LoaderBot(glm::vec3 startPos, glm::vec3 speed, int health, glm::vec3 dimension, ResourceManager* RM) {
	this->m_pos = startPos;
	this->m_accel = speed;
	this->m_health = health;
	this->m_dimension = dimension;
	
	// Create the hitbox of the enemy
	Box temp(this->m_pos, this->m_dimension.x, this->m_dimension.y, this->m_dimension.z);
	this->hitBox = temp;
}

LoaderBot::~LoaderBot() {
	// Empty
}

void LoaderBot::update(float deltaTime) {
	GameObject::update();
	
	if ((!dying) && (!canDie)) {
		this->hitBox.updateBoxPos(this->m_pos);
		checkHit();
		animate();
	}
	else {
		deathUpdate(deltaTime);
		animate();
	}
}