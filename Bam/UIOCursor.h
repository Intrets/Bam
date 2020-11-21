#pragma once

#include "ReferenceManager.h"
#include "Activity.h"
#include "UIOBase.h"
#include "UIOCallBackParams.h"

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
	UIOTextDisplay* hoveringText;
	UIOFreeSize* hoveringFreeElement;
	UIOBase* hoveringElement;

	Inventory& getInventory();
	ManagedReference<Activity, Activity> const& getTarget() const;

	UIOCursor(Handle self);
	~UIOCursor() = default;

	void setCursorWorldPosition(glm::vec2 p);
	void setCursorScreenPosition(glm::vec2 p);
	void clickWorld(UIOCallBackParams& params);
	void select(UIOCallBackParams& params, WeakReference<Activity, Activity> activity);
	void setWorldRender();

	virtual int32_t addRenderInfo(GameState& gameState, RenderInfo& renderInfo, int32_t depth) override;
	virtual ScreenRectangle updateSize(ScreenRectangle newScreenRectangle) override;
};

