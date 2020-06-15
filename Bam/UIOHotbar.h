#pragma once

#include "UIOBase.h"

class UIOButton;
class UIOShell;

class UIOHotbar : public UIOBase
{
public:
	std::vector<UIOButton*> slots;
	std::vector<UIOShell*> slotSize;

	int32_t selected = 0;

	UIOHotbar(Handle self);

	virtual ScreenRectangle updateSize(ScreenRectangle newScreenRectangle) override;
	virtual int32_t addRenderInfo(GameState& gameState, RenderInfo& renderInfo, int32_t depth) override;
};

