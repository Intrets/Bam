#pragma once

#include "ReferenceManager.h"
#include "Activity.h"
#include "UIOBase.h"
#include "UIOConstructActivityInterface.h"

class UIState;

class UIOActivityInterface : public UIOBase
{
public:
	enum class USER_ACTION_TYPE
	{
		NOTHING,
		HOVERING,
	};

private:
	ManagedReference<Activity, Activity> base;
	int32_t baseSelectionTick;

	ManagedReference<Activity, Activity> target;
	int32_t targetSelectionTick;

	UniqueReference<Activity, Activity> cursor;

	USER_ACTION_TYPE type;

public:
	UIOActivityInterface(Handle self);

	void exit();
	void cancel(bool full);
	void setBase(WeakReference<Activity, Activity> ref);
	void setTarget(WeakReference<Activity, Activity> ref);
	void splitTarget();
	void addLua(GameState& gameState, UIState& uiState);

	UIOActivityInterface::USER_ACTION_TYPE getType();

	void updateCursorPos(glm::vec2 pos);

	// Change the state of an actvity on cursor, like changing the length of a piston
	void changeHoverActivityState(int32_t type);

	void pickUp(GameState& gameState, glm::vec2 pos);
	void interact(GameState& gameState, glm::vec2 pos);
	void spawnHover(GameState& gameState, glm::ivec2 pos, Activity::TYPE activityType);
	void rotateHover(Activity::ROT rot);

	virtual int32_t addRenderInfo(GameState& gameState, RenderInfo& renderInfo, int32_t depth) override;

	virtual ScreenRectangle updateSize(ScreenRectangle newScreenRectangle) override;
};

