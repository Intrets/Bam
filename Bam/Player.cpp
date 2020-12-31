#include "common.h"

#include "Player.h"
#include "Inventory.h"

glm::vec2 Player::getCameraPosition() {
	return this->pos;
}

Inventory& Player::getInventory() {
	return this->inventory;
}
