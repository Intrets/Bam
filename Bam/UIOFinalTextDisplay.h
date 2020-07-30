#pragma once

#include "UIOBase.h"

#include "TextRenderInfo.h"

class UIOFinalTextDisplay : public UIOBase {
public:
	Text text;

	UIOFinalTextDisplay(Handle self, std::string text_);

	virtual ScreenRectangle updateSize(ScreenRectangle newScreenRectangle) override;
	virtual int32_t addRenderInfo(GameState& gameState, RenderInfo& renderInfo, int32_t depth) override;
};

