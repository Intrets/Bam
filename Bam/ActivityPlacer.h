#pragma once

#include "LogicSequencer.h"
#include "ReferenceManager.h"
#include "Activity.h"

enum class HOVERTYPES
{
	PLATFORM,
	PISTON,
	HOVERTYPES_MAX
};

class ActivityPlacer : public LogicSequencer
{
public:
	virtual void exit(GameState& gameState) override;

	WeakReference<Activity, Activity> hover;
	int hoverType;

	void placeHover(GameState& gameState, glm::ivec2 pos);
	void spawnHover(GameState& gameState, glm::ivec2 pos);

	ActivityPlacer();
	~ActivityPlacer() = default;

protected:
	virtual void appendRenderInfoInternal(GameState& gameState, RenderInfo& renderInfo) override;
};

