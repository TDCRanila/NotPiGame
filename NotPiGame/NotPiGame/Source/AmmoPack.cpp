#include "../Headers/AmmoPack.h"

#include "../Headers/ModelMatrix.h"
#include "../Headers/Player.h"

AmmoPack::AmmoPack(glm::vec3 pos, glm::vec3 dim, int ammoType, int ammoValue, Player* p) {
	this->m_pos			= pos;
	this->m_dimension	= dim;
	this->m_ammoType	= ammoType;
	this->m_ammoValue	= ammoValue;
	this->playerPointer = p;
	
	this->hitBox.width	= m_dimension.x;
	this->hitBox.height = m_dimension.y;
	this->hitBox.depth	= m_dimension.z;
	this->hitBox.pos	= m_pos;
	
	this->identifier = packIdentifier;
}

AmmoPack::~AmmoPack() {
	// Empty
}

void AmmoPack::update(float deltaTime) {
	GameObject::update();
	
	// Update the hitbox
	hitBox.updateBoxPos(this->m_pos);
	
	// "Animate" the pack
	this->animateTimer += deltaTime;
	float offsetSine = 0.75 * cos(animateTimer * 2);
	glm::vec3 p = this->m_model->getPositon();
	this->m_model->setPosition(glm::vec3(p.x, p.y + offsetSine, p.z));
	
	// Rotate them
	float offset	= animateTimer * 0.5;
	glm::vec3 r = this->m_model->getRotations();
	this->m_model->setRotations(glm::vec3(r.x, r.y + offset, r.z));
	
	// Check if the player has hit the pack
	checkHit();	
	
}

void AmmoPack::checkHit() {
	if (this->hitBox.AABBAABBIntersect(playerPointer->hitBox) && (pickUped == false)) {
		// Give information to the player
		playerPointer->storePickUpPointer(this);
		playerPointer->pickUpBool = true;
		
		// Now this pickup is no longer avaiable for pickup
		pickUped = true;
	}
}