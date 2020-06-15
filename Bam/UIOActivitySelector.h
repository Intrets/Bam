#pragma once

#include "UIOBase.h"
#include "Activity.h"

enum class SELECTION_TYPE
{
	NOTHING,
	HOVERING,
	SELECTED,
};

class UIOActivitySelector : public UIOBase
{
public:
	SELECTION_TYPE type = SELECTION_TYPE::NOTHING;

	std::vector<ManagedReference<Activity, Activity>> history;

	ManagedReference<Activity, Activity> target;

	void selectTarget(GameState& gameState);
	void expandTarget();
	void spawnHover(GameState& gameState, glm::ivec2 pos, ACTIVITY::TYPE activityType);

	UIOActivitySelector(Handle self);

	virtual ScreenRectangle updateSize(ScreenRectangle newScreenRectangle) override;

	virtual int32_t addRenderInfo(GameState& gameState, RenderInfo& renderInfo, int32_t depth);
};

