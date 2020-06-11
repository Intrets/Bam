#pragma once

#include "UIOBase.h"

class UIOTextDisplay : public UIOBase
{
public:
	bool hover;
	int32_t fontSize = 16;

	std::vector<std::string> text;

	UIOTextDisplay(Handle self);

	void setText(std::string text_);

	virtual ScreenRectangle updateSize(ScreenRectangle newScreenRectangle) override;
	virtual int32_t addRenderInfo(RenderInfo& renderInfo, int32_t depth) override;
};

