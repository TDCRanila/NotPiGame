#pragma once

#include "../Headers/Geometry.h"

#include "../Headers/GameObject.h"

class ResourceManager; class EnemyManager; class Gun; class Bullet; class Player; // FW Declaration

class Enemy : public GameObject {
public:
	friend EnemyManager;
	Enemy();
	virtual ~Enemy();

	void storePlayerPointer(Player* p);
	void storeEMPointer(EnemyManager* EM);
	
	virtual void update(float deltaTime) = 0;
	void deathUpdate(float deltaTime);
	
	void checkHit();
	void animate();
	
	Box hitBox;
	bool canDie			= false;
	bool dying			= false;
	
protected:

private:	
	// Animation Control
	bool animHurt		 = false;
	bool animHurtAnimate = false;
	bool animDead		 = false;
	bool animDeadAnimate = false;
	float animDeathTimer = false;
};