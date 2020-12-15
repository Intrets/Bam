#pragma once

#include "UIOBase.h"
#include "TextRenderInfo.h"

#include "Enums.h"

class UIOTextDisplay : public UIOBaseEnd
{
private:
	bool lineWrap = false;
	int32_t ticksSelected = 0;
	bool clickSupport = false;
	bool shrinkToFit = false;

public:
	UIOTEXTDISPLAY::MODE mode = UIOTEXTDISPLAY::MODE::NORMAL;

	Text text;

	UIOTextDisplay(Handle self);
	UIOTextDisplay(Handle self, bool lineWrap);

	virtual ~UIOTextDisplay() = default;

	virtual void translate(glm::vec2 p) override;

	virtual	CallBackBindResult runActiveBinds(State& state) override;

	void setMode(UIOTEXTDISPLAY::MODE mode_);

	void setShrinkToFit(bool b);
	void setClickSupport(bool b);

	void setWrap(bool b);

	void setText(std::string const& text_);
	void setText(std::vector<std::string> const& text_);

	void moveCursor(glm::ivec2 p);
	void setCursor(glm::ivec2 p);

	void moveStartWordForward();
	void moveStartWordBackward();

	void moveEndWord();

	bool yank(ControlState& controlState);
	bool paste(ControlState& controlState);

	void insertText(std::string text_);

	void backspaceChar();
	void deleteChar();

	virtual ScreenRectangle updateSize(ScreenRectangle newScreenRectangle) override;
	virtual int32_t addRenderInfo(GameState& gameState, RenderInfo& renderInfo, int32_t depth) override;
};

