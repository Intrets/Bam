#pragma once

#include "UIOBase.h"
#include "UIOButton.h"

class UIOWindow : public UIOBase
{
public:
	UIOBase* main;
	// TODO: class for segments like this
	UIOButton* topBar;
	UIOButton* bottomBar;
	UIOButton* rightBar;

	glm::vec2 mousePressedPosOffset;

	UIOWindow(Handle self, UniqueReference<UIOBase, UIOBase> main_);

	virtual ScreenRectangle updateSize(ScreenRectangle newScreenRectangle) override;
	virtual int32_t addRenderInfo(RenderInfo& renderInfo, int32_t depth) override;
};

