#pragma once

#include "UIOBase.h"

class UIOEmpty : public UIOBase
{
public:
	UIOEmpty(Handle self_);

	virtual ScreenRectangle updateSize(ScreenRectangle newScreenRectangle) override;

	virtual void addElement(UniqueReference<UIOBase, UIOBase> element) override;
	virtual void translate(glm::vec2 p) override;
	virtual void setScreenPixels(glm::ivec2 px) override;
	virtual int32_t addRenderInfo(GameState& gameState, RenderInfo& renderInfo, int32_t depth) override;
};

