#pragma once

#include "Block.h"

class Loader;
class Saver;
class GameState;

class ActivityMaterial
{
private:
	Material material;

	int32_t min;
	int32_t max;

	int32_t average;

public:
	int32_t getSmallRand(GameState& gameState) const;
	int32_t getVal() const;

	ActivityMaterial();
	ActivityMaterial(Loader& loader);
	ActivityMaterial(Material material_);
	~ActivityMaterial() = default;

	void save(Saver& saver);
	void load(Loader& loader);

};

