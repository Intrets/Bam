#pragma once

#include "UIOBase.h"
#include "TextRenderInfo.h"

class UIOTextDisplay : public UIOBaseEnd
{
private:
	bool lineWrap = false;
	int32_t ticksSelected = 0;
	bool clickSupport = false;
	bool shrinkToFit = false;

public:
	Text text;

	UIOTextDisplay(Handle self);
	UIOTextDisplay(Handle self, bool lineWrap);

	virtual void translate(glm::vec2 p) override;

	virtual	CallBackBindResult runActiveBinds(State& state) override;

	void setShrinkToFit(bool b);
	void setClickSupport(bool b);

	void setWrap(bool b);

	void setText(std::string text_);
	void setText(std::vector<std::string> text_);

	void moveCursor(glm::ivec2 p);
	void setCursor(glm::ivec2 p);

	void insertText(std::string text_);

	void backspaceChar();
	void deleteChar();

	virtual ScreenRectangle updateSize(ScreenRectangle newScreenRectangle) override;
	virtual int32_t addRenderInfo(GameState& gameState, RenderInfo& renderInfo, int32_t depth) override;
};

