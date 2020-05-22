#pragma once

#include "UIOBase.h"

class UIOScaling : public UIOBase
{
public:
	UIOBase* center;
	UIOBase* bottomBar;
	UIOBase* rightBar;

	UIOScaling(Handle self, UniqueReference<UIOBase, UIOBase> center_);

	virtual ScreenRectangle updateSize(ScreenRectangle newScreenRectangle) override;
	virtual int32_t addRenderInfo(RenderInfo& renderInfo, int32_t depth) override;
};

