#pragma once

#include "../Headers/Enemy.h"
#include "../Headers/StateMachine.h"
#include "../Headers/Bullet.h"

class TurretBot : public Enemy {
public:
	TurretBot(glm::vec3 startPos, glm::vec3 speed, int health, glm::vec3 dimension, ResourceManager* RM);
	~TurretBot();
	
	void update(float deltaTime);
	
protected:
	void updateRange();
	bool checkInSight();
	void shootBullet();
	
	// State Functions
	void gotoIdle();
	void idle();
	void gotoSearch();
	void search();
	void gotoShoot();
	void shoot();
	
	static void gotoIdleThunk(void * instance);
	static void OnIdleThunk(void * instance);
	static void gotoSearchingThunk(void * instance);
	static void OnSearchingThunk(void * instance);
	static void gotoShootingThunk(void * instance);
	static void OnShootingThunk(void * instance);
	
	State* gotoIdleState;
	State* onIdleState;
	State* gotoSearchingState;
	State* onSearchingState;
	State* gotoShootingState;
	State* onShootingState;
	
	StateMachine stateMachine;
private:
	// Game Stats
	int		turretDamage	= 10;
	float	turretBulletLifeTime = 1.25f;
	float	turretProjSpeed	= 50.f;
	float	turretRange		= 75.f;
	float	turretFireRate	= 1.5f;
	float	turretFireTimer	= 0.0f;
	
	float	targetDistance;
	bool	targetInRange = false;
	
	std::vector<TurretPlasma*> m_turretBullets;
	float	m_deltaTime;
	bool	m_goSearch	= true;;
};