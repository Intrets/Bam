#pragma once

#include "Inventory.h"

class Player
{
public:
	glm::vec2 pos = { 0.0f, 0.0f };

	Inventory inventory;

	glm::vec2 getCameraPosition();
	Inventory& getInventory();
};

