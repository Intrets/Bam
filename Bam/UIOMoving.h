#pragma once

#include "UIOBase.h"

class UIOButton;

class UIOMoving : public UIOBase
{
public:
	UIOBase* main;
	UIOButton* topBar;

	UIOMoving(Handle self, UniqueReference<UIOBase, UIOBase> main_);

	virtual ScreenRectangle updateSize(ScreenRectangle newScreenRectangle) override;
	virtual int32_t addRenderInfo(RenderInfo& renderInfo, int32_t depth) override;
};

