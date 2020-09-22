#include "common.h"

#include "ActivityMaterial.h"
#include "GameState.h"

int32_t ActivityMaterial::getSmallRand(GameState& gameState) const {
	return gameState.smallRandom.randRange(this->min, this->max);
}

int32_t ActivityMaterial::getVal() const {
	return this->average;
}

ActivityMaterial::ActivityMaterial() {
	this->min = 10;
	this->max = 15;
	this->average = 3;
}

ActivityMaterial::ActivityMaterial(Material material_) : material(material_) {
}
