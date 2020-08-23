#pragma once

#include "ReferenceManager.h"
#include "Activity.h"
#include "UIOBase.h"

class UIOActivityInterface : public UIOBase
{
private:
	ManagedReference<Activity, Activity> base;

public:
	UIOActivityInterface(Handle self);

	void setBase(WeakReference<Activity, Activity> ref);

	virtual int32_t addRenderInfo(GameState const& gameState, RenderInfo& renderInfo, int32_t depth) override;

	virtual ScreenRectangle updateSize(ScreenRectangle newScreenRectangle) override;
};

