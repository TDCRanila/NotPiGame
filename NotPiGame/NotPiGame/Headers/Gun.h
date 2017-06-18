#pragma once

#include "../Headers/Player.h"
#include "../Headers/Bullet.h"

#include <string>
#include <vector>

class MD2Model; class ResourceManager; class ModelMatrix; class Enemy; class Pistol; class Repeater; class Shotgun; // FW Declaration

class Gun {
	friend class Player;
public:
	Gun();
	virtual ~Gun();
	
	void storeGunModels(ModelMatrix* pointerGun, ModelMatrix* pointerFlash);
	ModelMatrix* getModel(int i);
	void storeEnemyVector(std::vector<Enemy*>* enemies);
	void storeGameModelVector(std::vector<ModelMatrix*>* gameModelVec);
	void storeResourceManager(ResourceManager* RM);
	void storePlayerPosition(glm::vec3 playerPos);
	void storePlayerFront(glm::vec3 playerFront);
	
	void shootEvent(float deltaTime);
	virtual void shootGun(float deltaTime) = 0;
	void resetGun(float deltaTime);
	void animate();
	
	bool canShoot = true;
	bool canSwitch = true;
	
	int getDamage();
	void setDamage(int d);
	
	int getAmmo();
	void setAmmo(int ammo);
	
protected:
	std::string m_weaponName;
	MD2Model* m_weaponModels[2];
	ResourceManager* m_resourceManager			= nullptr;
	std::vector<Enemy*>* m_enemyVectorPointer	= nullptr;
	std::vector<ModelMatrix*>* m_gameModelVec	= nullptr;
	
	glm::vec3 m_playerPosition;
	glm::vec3 m_playerFront;
	
	int		m_ammo;
	int		m_damage;
	float	m_fireRate;
	float	m_animSpeed;
	bool	m_penetration;
	float	m_gunTimer = 0;
};

class Pistol : public Gun {
public:
	Pistol(std::string name, int damage, float fireRate, int defaultAmmo, float animSpeed, bool penetration);
	~Pistol();
	
	void shootGun(float deltaTime);
	
private:
	
};

class Repeater : public Gun {
public:
	Repeater(std::string name, int damage, float fireRate, float projSpeed, int defaultAmmo, float animSpeed, bool penetration);
	~Repeater();
	
	void shootGun(float deltaTime);
	float repeaterBulletLifeTime	= 1.0f;
	std::vector<Bullet*> repeaterBullets;
	
private:
	glm::vec3 m_projectileSpeed;
};

class Shotgun : public Gun {
public:
	Shotgun(std::string name, int damage, float fireRate, int shotAmount, int defaultAmmo, float animSpeed, bool penetration);
	~Shotgun();
	
	void shootGun(float deltaTime);
	
private:
	int m_pelletsPerShot;
};