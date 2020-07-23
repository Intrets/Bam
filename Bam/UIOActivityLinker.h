#pragma once

#include "UIOBase.h"

class UIOActivityLinker : public UIOBase
{
public:
	UIOActivityLinker(Handle self);

	virtual ScreenRectangle updateSize(ScreenRectangle newScreenRectangle) override;
};

