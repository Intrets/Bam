#pragma once

#include "UIOGrid.h"

class Inventory;
class UIOTextDisplay;

class UIOInventory : public UIOGrid
{
private:
	Inventory& getInventory();

	std::vector<UIOTextDisplay*> icons;

public:
	UIOInventory(Handle self);

	virtual int32_t addRenderInfo(GameState& gameState, RenderInfo& renderInfo, int32_t depth) override;
};

