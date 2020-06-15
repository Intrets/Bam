#pragma once

#include "UIOBase.h"

class UIOBasicWindow : public UIOBase
{
public:
	UIOBasicWindow(Handle self);

	virtual ScreenRectangle updateSize(ScreenRectangle newScreenRectangle) override;
	virtual int32_t addRenderInfo(GameState& gameState, RenderInfo& renderInfo, int32_t depth) override;
};

