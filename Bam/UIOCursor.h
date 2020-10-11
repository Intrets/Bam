#pragma once

#include "ReferenceManager.h"
#include "Activity.h"
#include "UIOBase.h"
#include "UIOCallBackParams.h"

class Inventory;
class UIOFreeSize;
class UIOConstrainSize;
class UIOTextDisplay;

class UIOCursor : public UIOBase
{
private:
	glm::vec2 cursorWorldPosition = glm::vec2(0, 0);

	bool renderInWorld = false;

	int32_t selectionTick = 0;

	UIOTextDisplay* hoveringText;
	UIOFreeSize* hoveringFreeElement;
	UIOBase* hoveringElement;

	ManagedReference<Activity, Activity> target;

	UIOCursor() = default;

public:
	Inventory& getInventory();

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

