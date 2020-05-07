#pragma once

#include "LogicSequencer.h"
#include "ReferenceManager.h"
#include "Activity.h"

class ActivityLinker : public LogicSequencer
{
public:
	WeakReference<Activity, Activity> target;

	ActivityLinker();
	~ActivityLinker() = default;

protected:
	virtual void appendRenderInfoInternal(GameState& gameState, RenderInfo& renderInfo) override;
};

