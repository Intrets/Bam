#pragma once

#include "UIOBase.h"

class Inventory;
class UIOFreeSize;
class UIOConstrainSize;
class UIOTextDisplay;

class UIOCursor : public UIOBase
{
private:
	Inventory& getInventory();
	glm::vec2 cursorWorldPosition;

	bool renderInWorld = false;

	UIOTextDisplay* hoveringText;
	UIOFreeSize* hoveringFreeElement;
	UIOBase* hoveringElement;

	UIOCursor() = default;

public:
	UIOCursor(Handle self);
	~UIOCursor() = default;

	void setCursorWorldPosition(glm::vec2 p);
	void setCursorScreenPosition(glm::vec2 p);
	void setWorldRender();

	virtual int32_t addRenderInfo(GameState& gameState, RenderInfo& renderInfo, int32_t depth) override;
	virtual ScreenRectangle updateSize(ScreenRectangle newScreenRectangle) override;
};

