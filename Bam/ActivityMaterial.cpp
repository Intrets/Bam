#include "common.h"

#include "ActivityMaterial.h"
#include "GameState.h"
#include "Saver.h"
#include "Loader.h"

int32_t ActivityMaterial::getSmallRand(GameState& gameState) const {
	return gameState.smallRandom.randRange(this->min, this->max);
}

int32_t ActivityMaterial::getVal() const {
	return this->average;
}

ActivityMaterial::ActivityMaterial() {
	this->min = 10;
	this->max = 15;
	this->average = 12;
}

ActivityMaterial::ActivityMaterial(Loader& loader) {
	this->load(loader);
}

ActivityMaterial::ActivityMaterial(Material material_) : material(material_) {
}

void ActivityMaterial::save(Saver& saver) {
	this->material.save(saver);
}

void ActivityMaterial::load(Loader& loader) {
	this->material.load(loader);
}
