#pragma once

#include "ReferenceManager.h"
#include "Activity.h"
#include "UIOBase.h"
#include "UIOConstructActivityInterface.h"

class UIState;

namespace UIO
{
	enum class USER_ACTION_TYPE
	{
		NOTHING,
		HOVERING,
	};

}

class UIOActivityInterface : public UIOBase
{
private:
	ManagedReference<Activity, Activity> target;
	int32_t targetSelectionTick;
	int32_t baseSelectionTick;

	UniqueReference<Activity, Activity> cursor;

	UIO::USER_ACTION_TYPE type;

public:
	UIOActivityInterface(Handle self);

	void exit();
	void cancel(bool full);
	void setTarget(WeakReference<Activity, Activity> ref);
	void splitTarget();

	UIO::USER_ACTION_TYPE getType();

	void updateCursorPos(glm::vec2 pos);

	// Change the state of an actvity on cursor, like changing the length of a piston
	void changeHoverActivityState(int32_t type);

	void pickUp(GameState& gameState, glm::vec2 pos);
	void interact(GameState& gameState, glm::vec2 pos);
	void spawnHover(GameState& gameState, glm::ivec2 pos, ACTIVITY::TYPE activityType);
	void rotateHover(ACTIVITY::ROT rot);

	virtual int32_t addRenderInfo(GameState& gameState, RenderInfo& renderInfo, int32_t depth) override;

	virtual ScreenRectangle updateSize(ScreenRectangle newScreenRectangle) override;
};

