#pragma once

#include "../Headers/Pickup.h"

class HealthPack : public Pickup {
public:
	HealthPack(glm::vec3 pos, glm::vec3 dim, int healthValue, Player* player);
	~HealthPack();
	
	void update(float deltaTime);
	void checkHit();
	
	int m_healthValue;
private:
	int packIdentifier = 1;	
};