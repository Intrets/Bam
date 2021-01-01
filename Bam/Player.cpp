#include "common.h"

#include "Player.h"
#include "Inventory.h"
#include "Saver.h"
#include "Loader.h"

glm::vec2 Player::getCameraPosition() {
	return this->pos;
}

Inventory& Player::getInventory() {
	return this->inventory;
}

bool Player::load(Loader& loader) {
	loader.retrieve(this->pos);

	this->inventory.load(loader);
	return true;
}

bool Player::save(Saver& saver) {
	saver.store(this->pos);

	this->inventory.save(saver);
	return true;
}
