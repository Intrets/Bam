#pragma once

#include "UIOBase.h"

class UIOInvisible : public UIOBase
{
public:
	UIOInvisible(Handle self);

	virtual ScreenRectangle updateSize(ScreenRectangle newScreenRectangle) override;
};

