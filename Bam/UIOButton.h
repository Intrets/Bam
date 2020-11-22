#pragma once

#include "UIOBase.h"

#include "Colors.h"

class UIOButton : public UIOBaseSingle
{
private:
	bool shrinkToFit = true;
	bool highlighted;

	glm::vec4 color = COLORS::UI::BACKGROUND;

	bool down = false;
	glm::vec2 mousePressOffset;

private:
	template<class T>
	friend class UIOConstructer;

	friend class UIOHotbar;

	CallBack onPress;
	CallBack onRelease;

public:
	void setColor(glm::vec4 c);

	void setOnPress(CallBack f);
	void setOnRelease(CallBack f);
	void setShrinkToFit(bool b);

	bool isDown();
	glm::vec2 const& getMousePressOffset() const;

	UIOButton(Handle self);
	virtual ~UIOButton() = default;

	virtual ScreenRectangle updateSize(ScreenRectangle newScreenRectangle) override;
	virtual int32_t addRenderInfo(GameState& gameState, RenderInfo& renderInfo, int32_t depth) override;
};

