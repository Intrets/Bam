#pragma once

#include "UIOBase.h"
#include "TextRenderInfo.h"

class UIOTextEdit : public UIOBase
{
public:
	bool focus;
	
	Text text;

	UIOTextEdit(Handle self);

	virtual void translate(glm::vec2 p) override;

	virtual ScreenRectangle updateSize(ScreenRectangle newScreenRectangle) override;
	virtual int32_t addRenderInfo(GameState& gameState, RenderInfo& renderInfo, int32_t depth) override;

	void moveCursor(glm::ivec2 p);
	void insertText(std::string text);
	void backspaceChar();
	void deleteChar();
};

