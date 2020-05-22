#pragma once

#include "UIOBase.h"

class UIOButton;

class UIOScaling : public UIOBase
{
public:
	bool scaling;

	UIOBase* center;
	UIOButton* bottomBar;
	UIOButton* rightBar;

	UIOScaling(Handle self, UniqueReference<UIOBase, UIOBase> center_);

	virtual ScreenRectangle updateSize(ScreenRectangle newScreenRectangle) override;
	virtual int32_t addRenderInfo(RenderInfo& renderInfo, int32_t depth) override;
};
