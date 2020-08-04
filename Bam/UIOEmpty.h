#pragma once

#include "UIOBase.h"

class UIOEmpty :public UIOBase
{
public:
	UIOEmpty(Handle self_);
	virtual ScreenRectangle updateSize(ScreenRectangle newScreenRectangle) override;
};

