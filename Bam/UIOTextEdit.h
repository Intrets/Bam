#pragma once

#include "UIOBase.h"

class UIOTextEdit : public UIOBase
{
public:
	bool focus;

	std::vector<std::string> lines;
	glm::ivec2 cursor;

	glm::ivec2 viewHorizontal = { 0,30 };
	glm::ivec2 viewVertical = { 0,30 };

	glm::ivec2 viewSize;
	float fontWidth;

	UIOTextEdit(Handle self);

	virtual ScreenRectangle updateSize(ScreenRectangle newScreenRectangle) override;
	virtual int32_t addRenderInfo(GameState& gameState, RenderInfo& renderInfo, int32_t depth) override;

	void moveCursor(glm::ivec2 p);
	void insertText(std::string text);
	void backspaceChar();
	void deleteChar();
};

