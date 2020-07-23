#pragma once

#include "UIOBase.h"
#include "TextRenderInfo.h"	

class UIOTextDisplay : public UIOBase
{
public:
	std::vector<std::string> text;

	std::optional<WindowTextRenderInfo> cachedText;

	UIOTextDisplay(Handle self);

	void setText(std::string text_);
	void addLine(std::string text_);

	virtual ScreenRectangle updateSize(ScreenRectangle newScreenRectangle) override;
	virtual int32_t addRenderInfo(GameState& gameState, RenderInfo& renderInfo, int32_t depth) override;
};

