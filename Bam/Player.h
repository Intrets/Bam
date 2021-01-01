#pragma once

#include "Inventory.h"

class Loader;
class Saver;

class Player
{
public:
	glm::vec2 pos = { 0.0f, 0.0f };

	Inventory inventory;

	glm::vec2 getCameraPosition();
	Inventory& getInventory();

	bool load(Loader& loader);
	bool save(Saver& saver);
};

