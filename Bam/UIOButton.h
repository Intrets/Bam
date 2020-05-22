#pragma once

#include "UIOBase.h"


static bool onPress(GameState& gameState, ControlState& controlState, UIOBase* self);
static bool onRelease(GameState& gameState, ControlState& controlState, UIOBase* self);

class UIOButton : public UIOBase
{
public:
	bool down = false;

	std::vector<CallBack> whileDown;

	UIOButton(Handle self);

	virtual ScreenRectangle updateSize(ScreenRectangle newScreenRectangle) override;
	virtual int32_t addRenderInfo(RenderInfo& renderInfo, int32_t depth) override;
};

