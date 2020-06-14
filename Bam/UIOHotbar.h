#pragma once

#include "UIOBase.h"

class UIOHotbar : public UIOBase
{
public:
	std::vector<UIOBase*> slots;
	int32_t selected = 0;

	UIOHotbar(Handle self);

	virtual ScreenRectangle updateSize(ScreenRectangle newScreenRectangle) override;
	virtual int32_t addRenderInfo(RenderInfo& renderInfo, int32_t depth) override;
};

