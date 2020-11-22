#pragma once

#include "UIOBase.h"

class UIOInvisible : public UIOBaseMulti
{
public:
	UIOInvisible(Handle self);
	virtual ~UIOInvisible() = default;

	virtual ScreenRectangle updateSize(ScreenRectangle newScreenRectangle) override;
};

