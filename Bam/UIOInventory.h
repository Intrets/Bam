#pragma once

#include <vector>

#include "UIOGrid.h"

class Inventory;
class UIOTextDisplay;

class UIOInventory : public UIOGrid
{
private:
	std::vector<UIOTextDisplay*> icons;

public:
	UIOInventory(Handle self);
	virtual ~UIOInventory() = default;

	virtual int32_t addRenderInfo(GameState& gameState, RenderInfo& renderInfo, int32_t depth) override;
};

