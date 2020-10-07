#pragma once

#include "UIOGrid.h"

class Inventory;
class UIOTextDisplay;

class UIOHotbar2 : public UIOGrid
{
private:
	Inventory& getInventory();

	std::vector<UIOTextDisplay*> icons;

public:
	UIOHotbar2(Handle self);

	virtual int32_t addRenderInfo(GameState& gameState, RenderInfo& renderInfo, int32_t depth) override;
};

