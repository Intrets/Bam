#pragma once

#include "ReferenceManager.h"
#include "Activity.h"
#include "UIOBase.h"

class UIOActivityInterface : public UIOBase
{
private:
	ManagedReference<Activity, Activity> base;

public:
	virtual int32_t addRenderInfo(GameState& gameState, RenderInfo& renderInfo, int32_t depth);

	virtual ScreenRectangle updateSize(ScreenRectangle newScreenRectangle) override;
};

