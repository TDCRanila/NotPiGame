#include "../Headers/EnemyManager.h"

#include "../Headers/ModelMatrix.h"
#include "../Headers/ResourceManager.h"

#include "../Headers/Enemy.h"
#include "../Headers/Player.h"

EnemyManager::EnemyManager() {
	// ALlocate the enemy vector
	m_enemies = new std::vector<Enemy*>;
}

EnemyManager::~EnemyManager() {
	deleteEnemies();
}
	
void EnemyManager::storePlayerPointer(Player* p) {
	this->playerPointer = p;
}

void EnemyManager::storeRMPointer(ResourceManager* RM) {
	this->m_resourceManagerPointer = RM;
}
void EnemyManager::storeGameModels(std::vector<ModelMatrix*>* gameModels) {
	this->m_gameModelPointer = gameModels;
}

void EnemyManager::updateManager(float deltaTime) {
	// Store the deltaTime
	this->m_deltaTime = deltaTime;
	
	// Check for all the enemies
	if (!m_enemies->empty()) {
		for (int enemies = 0; enemies < m_enemies->size(); enemies++) {
			Enemy* enemy = (*m_enemies)[enemies];
			enemy->storePlayerPointer(playerPointer);
			enemy->storeEMPointer(this);
		
			// If the enemy is still alive then update it 
			if (!enemy->getDeadBool()) {
				updateEnemies(enemies);
			} else {
				updateDeadEnemies(enemies);
			}
	} }
}

void EnemyManager::addEnemy(Enemy* enemy) {
	m_enemies->push_back(enemy);
}

void EnemyManager::deleteEnemies() {
	// Delete the contents of the vetor
	for (int i = 0; i < m_enemies->size(); i++) {
		delete (*m_enemies)[i];
		(*m_enemies)[i] = nullptr;
	}
	
	// And now delete the vector
	delete m_enemies;
}

void EnemyManager::updateEnemies(int i) {
	Enemy* enemy = (*m_enemies)[i];
	enemy->update(m_deltaTime);
}

void EnemyManager::updateDeadEnemies(int i) {
	// When the enemy is dead the remove it
	(*m_enemies)[i]->getModel()->deleteModel = true;
	delete (*m_enemies)[i];
	(*m_enemies)[i] = m_enemies->back();
	m_enemies->pop_back();
}
		
std::vector<Enemy*>* EnemyManager::getEnemyVector() {
	return this->m_enemies;
}