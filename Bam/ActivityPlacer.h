#pragma once

#include "ReferenceManager.h"
#include "Activity.h"
#include "ActivitySelector.h"

enum class HOVERTYPES
{
	PLATFORM,
	PISTON,
	ACTIVATOR,
	CRANE,
	HOVERTYPES_MAX
};

class ActivityPlacer : public ActivitySelector 
{
public:
	virtual void exit(GameState& gameState) override;
	
	virtual void selectTarget(GameState& gameState) override;

	WeakReference<Activity, Activity> hover;
	int hoverType;

	void spawnHover(GameState& gameState, glm::ivec2 pos);
	void rotateHover(MOVEABLE::ROT rot);
	void deleteHover();

	ActivityPlacer();
	~ActivityPlacer() = default;

protected:
	virtual void appendRenderInfoInternal(GameState& gameState, RenderInfo& renderInfo) override;
};

