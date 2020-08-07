#pragma once

#include "UIOBase.h"

#include "TextRenderInfo.h"
#include "Colors.h"

class UIOSimpleTextDisplay : public UIOBase {
private:
	Text text;

public:
	void updateText(std::string text_);
	void updateText(std::vector<std::string> lines);

	virtual void translate(glm::vec2 p) override;

	UIOSimpleTextDisplay(Handle self, std::string text_);
	UIOSimpleTextDisplay(Handle self, std::vector<std::string> lines);

	virtual ScreenRectangle updateSize(ScreenRectangle newScreenRectangle) override;
	virtual int32_t addRenderInfo(GameState& gameState, RenderInfo& renderInfo, int32_t depth) override;
};

