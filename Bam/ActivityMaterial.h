#pragma once

#include "Block.h"

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
	ActivityMaterial(Material material_);
	~ActivityMaterial() = default;
};

