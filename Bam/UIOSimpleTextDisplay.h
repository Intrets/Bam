#pragma once

#include "UIOBase.h"

#include "TextRenderInfo.h"

class UIOSimpleTextDisplay : public UIOBase {
public:
	// TODO: stop cursor from rendering in Text
	Text text;

	virtual void translate(glm::vec2 p) override;

	UIOSimpleTextDisplay(Handle self, std::string text_);

	virtual ScreenRectangle updateSize(ScreenRectangle newScreenRectangle) override;
	virtual int32_t addRenderInfo(GameState& gameState, RenderInfo& renderInfo, int32_t depth) override;
};

