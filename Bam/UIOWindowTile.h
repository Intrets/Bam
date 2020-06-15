#pragma once

#include "UIOBase.h"

class UIOWindowTile : public UIOBase
{
public:
	std::vector<std::vector<UIOBase*>> rows;

	void nextRow();
	void add(UniqueReference<UIOBase, UIOBase> c);

	UIOWindowTile(Handle self);

	virtual ScreenRectangle updateSize(ScreenRectangle newScreenRectangle) override;
	virtual int32_t addRenderInfo(GameState& gameState, RenderInfo& renderInfo, int32_t depth) override;
};

