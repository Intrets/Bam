#pragma once

#include "UIOBase.h"
#include "Activity.h"

class UIOActivitySelector : public UIOBase
{
public:
	UIOActivitySelector(Handle self);

	virtual ScreenRectangle updateSize(ScreenRectangle newScreenRectangle) override;

	virtual int32_t addRenderInfo(GameState& gameState, RenderInfo& renderInfo, int32_t depth) override;
};

