#pragma once

#include "../Headers/GameObject.h"
#include "../Headers/Geometry.h"

#include <string>

class Player; // FW Declaration

class Pickup : public GameObject {
public:
	friend class PickupManager;
	Pickup();
	virtual ~Pickup();
	
	void virtual update(float deltaTime) = 0;
	void virtual checkHit() = 0;
	
	Box hitBox;
	int identifier		= -1;
protected:
	float		animateTimer	= 0;
	bool		pickUped		= false;
	Player*		playerPointer	= nullptr;
};

class PickupManager {
public:
	PickupManager();
	~PickupManager();
	
	void updatePickups(float deltaTime);
	void addPickup(Pickup* pickup);

protected:
	std::vector<Pickup*> pickups;
};
