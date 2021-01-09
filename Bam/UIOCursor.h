#pragma once

#include <GLM/glm.hpp>

#include "ReferenceManager.h"
#include "Activity.h"
#include "UIOBase.h"
#include "PlayerState.h"

class Inventory;
class UIOFreeSize;
class UIOConstrainSize;
class UIOTextDisplay;

class UIOCursor : public UIOBaseMulti
{
private:
	glm::vec2 cursorWorldPosition = glm::vec2(0, 0);

	bool renderInWorld = false;

	int32_t selectionTick = 0;

	ManagedReference<Activity, Activity> target;

	UIOCursor() = default;

public:
	WeakReference<UIOBase, UIOTextDisplay> hoveringText;
	WeakReference<UIOBase, UIOFreeSize> hoveringFreeElement;
	WeakReference<UIOBase, UIOBase> hoveringElement;

	ManagedReference<Activity, Activity> const& getTarget() const;

	UIOCursor(Handle self);
	virtual ~UIOCursor() = default;

	void setCursorWorldPosition(glm::vec2 p);
	void setCursorScreenPosition(glm::vec2 p);
	void clickWorld(PlayerState& playerState);
	void select(PlayerState& playerState, WeakReference<Activity, Activity> activity);
	void setWorldRender();

	virtual int32_t addRenderInfo(GameState& gameState, RenderInfo& renderInfo, int32_t depth) override;
	virtual ScreenRectangle updateSize(ScreenRectangle newScreenRectangle) override;
};

