#pragma once

#include "../Headers/Pickup.h"

class AmmoPack : public Pickup {
public:
	AmmoPack(glm::vec3 pos, glm::vec3 dim, int ammoType, int ammoValue, Player* p);
	~AmmoPack();
	
	void update(float deltaTim);
	void checkHit();
	
	int m_ammoType;
	int m_ammoValue;
	
private:
	int packIdentifier = 2;
};