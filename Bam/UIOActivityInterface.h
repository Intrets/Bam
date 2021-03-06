#pragma once

#include "ReferenceManager.h"
#include "Activity.h"
#include "UIOBase.h"

class UIState;

namespace UIO
{
	enum class USER_ACTION_TYPE
	{
		NOTHING,
		HOVERING,
	};
}

class UIOActivityInterface : public UIOBaseEnd
{
private:
	ManagedReference<Activity, Activity> target;
	int32_t targetSelectionTick = 0;
	int32_t baseSelectionTick = 0;

	UniqueReference<Activity, Activity> cursor;

	UIO::USER_ACTION_TYPE type = UIO::USER_ACTION_TYPE::NOTHING;

public:
	UIOActivityInterface(Handle self);
	virtual ~UIOActivityInterface() = default;

	void exit();
	void cancel(bool full);
	void setTarget(WeakReference<Activity, Activity> ref);
	void splitTarget();
	ManagedReference<Activity, Activity> const& getTarget();

	UIO::USER_ACTION_TYPE getType();

	void updateCursorPos(glm::vec2 pos);

	// Change the state of an actvity on cursor, like changing the length of a piston
	void changeHoverActivityState(int32_t type);

	void copy(GameState& gameState, glm::vec2 pos);
	void pickUp(GameState& gameState, glm::vec2 pos);
	void interact(GameState& gameState, glm::vec2 pos);
	void spawnHover(GameState& gameState, glm::ivec2 pos, ACTIVITY::TYPE activityType);
	void rotateHover(ACTIVITY::ROT rot);

	virtual int32_t addRenderInfo(GameState& gameState, RenderInfo& renderInfo, int32_t depth) override;

	virtual ScreenRectangle updateSize(ScreenRectangle newScreenRectangle) override;
};

