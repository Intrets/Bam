#pragma once

#include "LogicSequencer.h"
#include "ReferenceManager.h"
#include "Activity.h"

class ActivityPlacer : public LogicSequencer
{
public:
	WeakReference<Activity, Activity> hover;
	void placeHover(GameState& gameState, glm::ivec2 pos);

	ActivityPlacer();
	~ActivityPlacer() = default;

protected:
	virtual void appendRenderInfoInternal(GameState& gameState, RenderInfo& renderInfo) override;
};

