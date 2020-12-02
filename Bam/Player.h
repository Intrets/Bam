#pragma once

class Inventory;

class Player
{
public:
	glm::vec2 pos;

	glm::vec2 getCameraPosition();
	Inventory& getInventory();
};

