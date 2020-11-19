#pragma once

#include "UIOBase.h"

class UIOInvisible : public UIOBaseMulti
{
public:
	UIOInvisible(Handle self);

	virtual ScreenRectangle updateSize(ScreenRectangle newScreenRectangle) override;
};

