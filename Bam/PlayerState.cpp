#include "common.h"

#include "PlayerState.h"

Player& PlayerState::getPlayer() {
	return *this->player;
}
