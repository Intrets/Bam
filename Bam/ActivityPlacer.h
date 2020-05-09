#pragma once

#include "ReferenceManager.h"
#include "Activity.h"
#include "ActivitySelector.h"

enum class HOVERTYPES
{
	PLATFORM,
	PISTON,
	HOVERTYPES_MAX
};

class ActivityPlacer : public ActivitySelector 
{
public:
	virtual void exit(GameState& gameState) override;

	WeakReference<Activity, Activity> hover;
	int hoverType;

	void spawnHover(GameState& gameState, glm::ivec2 pos);

	ActivityPlacer();
	~ActivityPlacer() = default;

protected:
	virtual void appendRenderInfoInternal(GameState& gameState, RenderInfo& renderInfo) override;
};

