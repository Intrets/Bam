#pragma once

#include "UIOBase.h"
#include "Rectangle.h"

class UIOFreeSize : public UIOBaseSingle
{
public:
	UIOFreeSize(Handle self);
	virtual ~UIOFreeSize() = default;

	virtual ScreenRectangle updateSize(ScreenRectangle newScreenRectangle) override;
};

