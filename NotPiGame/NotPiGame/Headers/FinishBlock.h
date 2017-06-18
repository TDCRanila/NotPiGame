#pragma once

#include "../Headers/Pickup.h"

class FinishBlock : public Pickup {
public:
	FinishBlock(glm::vec3 pos, glm::vec3 dim, Player* player);
	~FinishBlock();
	
	void update(float deltaTime);
	void checkHit();

private:
	int packIdentifier = 0;	
};