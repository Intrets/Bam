#pragma once

#include "ReferenceManager.h"
#include "Activity.h"
#include "UIOBase.h"

class UIOActivityInterface : public UIOBase
{
public:
	enum class USER_ACTION_TYPE 
	{
		NOTHING,
		HOVERING,
		SELECTED,
	};

private:
	ManagedReference<Activity, Activity> base;
	int32_t baseSelectionTick;

	ManagedReference<Activity, Activity> target;
	int32_t targetSelectionTick;

	ManagedReference<Activity, Activity> cursor;

public:
	USER_ACTION_TYPE type;

	UIOActivityInterface(Handle self);

	void setBase(WeakReference<Activity, Activity> ref);
	void setTarget(WeakReference<Activity, Activity> ref);

	void updateCursorPos(glm::vec2 pos);

	void interact(GameState& gameState, glm::vec2 pos);
	void spawnHover(GameState& gameState, glm::ivec2 pos, Activity::TYPE activityType);
	void rotateHover(Activity::ROT rot);

	virtual int32_t addRenderInfo(GameState const& gameState, RenderInfo& renderInfo, int32_t depth) override;

	virtual ScreenRectangle updateSize(ScreenRectangle newScreenRectangle) override;
};

