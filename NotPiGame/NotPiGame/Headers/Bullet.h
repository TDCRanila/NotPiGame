#pragma once

#include "../Headers/GameObject.h"

#include "../glm/include/common.hpp"

class Bullet : public GameObject {
public:
	Bullet();
	virtual ~Bullet();
	
	virtual void update(float deltaTime) = 0;
	int getDamage();
	void setDamage(int d);
	
protected:
	
	glm::vec3 bulletDirection;
	int bulletDamage;
	float lifeTime = 0.0f;
	float maxLifeTime;
};

class RepeaterPlasma : public Bullet {
public:
	RepeaterPlasma(glm::vec3 pos, glm::vec3 speed, int bulletDamage, float bulletLifeTime, glm::vec3 bulletDirection);
	~RepeaterPlasma();
	
	void update(float deltaTime);
};

class TurretPlasma : public Bullet {
public:
	TurretPlasma(glm::vec3 pos, glm::vec3 speed, int bulletDamage, float bulletLifeTime, glm::vec3 bulletDirection);
	~TurretPlasma();

	void update(float deltaTime);
};