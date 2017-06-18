#include "../Headers/Pickup.h"

#include "../Headers/Player.h"
#include "../Headers/ModelMatrix.h"

Pickup::Pickup() {
	// Empty
}

Pickup::~Pickup() {
	// Empty
}

PickupManager::PickupManager() {
	// Empty	
}

PickupManager::~PickupManager() {
	// Delete all the pickups
	for (int i = 0; i < pickups.size(); i++) {
		delete pickups[i];
		pickups[i] = nullptr;
	}
}

void PickupManager::addPickup(Pickup* pickup) {
	pickups.push_back(pickup);
}

void PickupManager::updatePickups(float deltaTime) {
	
	for (int i = 0; i < pickups.size(); i++) {
		// Update all the pickups
		pickups[i]->update(deltaTime);
		
		// Check if there are pickups that need to be deleted
		if (pickups[i]->pickUped) {
			// When the enemy is dead the remove it
			pickups[i]->getModel()->deleteModel = true;
			delete pickups[i];
			pickups[i] = pickups.back();
			pickups.pop_back();
		}
	}
}