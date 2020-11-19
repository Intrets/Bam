#pragma once

#include "UIOBase.h"

class UIOGrid : public UIOBaseMulti
{
private:
	glm::ivec2 size;

public:
	UIOGrid() = default;
	UIOGrid(Handle self, glm::ivec2 size_);
	virtual	~UIOGrid() = default;

	virtual ScreenRectangle updateSize(ScreenRectangle newScreenRectangle) override;

	virtual int32_t addRenderInfo(GameState& gameState, RenderInfo& renderInfo, int32_t depth) override;
};

