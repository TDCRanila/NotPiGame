#pragma once

#include "../Headers/Enemy.h"
#include "../Headers/StateMachine.h"

class LoaderBot : public Enemy {
public:
	LoaderBot(glm::vec3 startPos, glm::vec3 speed, int health, glm::vec3 dimension, ResourceManager* RM);
	~LoaderBot();
	
	void update(float deltaTime);
		
protected:
	
	StateMachine m_stateMachine;
};
