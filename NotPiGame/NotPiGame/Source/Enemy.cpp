#include "../Headers/Enemy.h"

#include "../Headers/ResourceManager.h"
#include "../Headers/EnemyManager.h"
#include "../Headers/MD2Model.h"
#include "../Headers/Gun.h"
#include "../Headers/Bullet.h"
#include "../Headers/Player.h"

#include "../Headers/Utilities.h"

Enemy::Enemy() {
	// Empty
}

Enemy::~Enemy() {

}

void Enemy::storePlayerPointer(Player* p) {
	this->playerPointer = p;
}

void Enemy::storeEMPointer(EnemyManager* em) {
	this->EMPointer = em;
}

void Enemy::deathUpdate(float deltaTime) {
	if (animDeathTimer > 5.f) {
		this->setDeadBool(true);
	} else {
		animDeathTimer += deltaTime;
		
		glm::vec3 tempPos = this->getPos();
		this->setPos(glm::vec3(tempPos.x, tempPos.y - 0.0625f, tempPos.z));
		animate();
	}
}

void Enemy::checkHit() {
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
		animHurt = true;
		hits = 0;
		
		// There is no more hit
		this->m_hit = false;
		
		// If the enemy has no more health left - declare it dead
		if (this->getHealth() <= 0) {
			dying = true;
			animHurt = false;
			animHurtAnimate = false;
			animDead = true;
		}
	}
}

void Enemy::animate() {
	MD2Model* model = dynamic_cast<MD2Model*>(this->getModel());
		
	if (animDeadAnimate) {
		if (model->getFrameStep() + 1 >= model->modelPackage.frameStates[model->getFrameState()]->amountOfFrames - 1) {
			model->setAnimate(false);
			animDeadAnimate = false;
			canDie = true;
			return;
	} }
	
	if (animDead) {
		int randomAnim = Utilities::randomIntNumRange(17, 19);
		model->setFrameState(randomAnim);
		model->setAnimate(true);
		animDead = false;
		animHurtAnimate = false;
		animDeadAnimate = true;
	}
		
	if (animHurtAnimate) {
		if (model->getFrameStep() + 1 >= model->modelPackage.frameStates[model->getFrameState()]->amountOfFrames - 1) {
			model->setFrameState(0); // Set animation back to standing
			animHurtAnimate = false;
			return;
	} }
	
	if (animHurt) {
		int randomAnim = Utilities::randomIntNumRange(3, 5);
		model->setFrameState(randomAnim);
		model->setAnimate(true);
		animHurt = false;
		animHurtAnimate = true;
	}
}