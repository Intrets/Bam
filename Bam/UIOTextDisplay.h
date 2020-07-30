#pragma once

#include "UIOBase.h"
#include "TextRenderInfo.h"	

class UIOTextDisplay : public UIOBase
{
public:
	Text text;

	UIOTextDisplay(Handle self);

	void moveCursor(glm::ivec2 p);
	void insertText(std::string text);
	void backspaceChar();
	void deleteChar();

	virtual ScreenRectangle updateSize(ScreenRectangle newScreenRectangle) override;
	virtual int32_t addRenderInfo(GameState& gameState, RenderInfo& renderInfo, int32_t depth) override;
};

