#pragma once

#include <vector>

class Enemy; class Player; class ResourceManager; class ModelMatrix; // FW Declaration

class EnemyManager {
public:
	EnemyManager();
	~EnemyManager();
	
	void storePlayerPointer(Player* p);
	void storeRMPointer(ResourceManager* RM);
	void storeGameModels(std::vector<ModelMatrix*>* gameModels);
	
	void updateManager(float DeltaTime);
	void addEnemy(Enemy* enemy);
	void deleteEnemies();
	
	void updateEnemies(int i);
	void updateDeadEnemies(int i);
	
	std::vector<Enemy*>* getEnemyVector();
	
	Player* playerPointer;
	std::vector<Enemy*>*		m_enemies					= nullptr;
	ResourceManager*			m_resourceManagerPointer	= nullptr;
	std::vector<ModelMatrix*>*	m_gameModelPointer			= nullptr;
	float m_deltaTime;
};