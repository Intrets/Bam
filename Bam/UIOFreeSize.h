#pragma once

#include "UIOBase.h"

class UIOFreeSize : public UIOBase
{
public:
	UIOFreeSize(Handle self, UniqueReference<UIOBase, UIOBase> main);

	virtual ScreenRectangle updateSize(ScreenRectangle newScreenRectangle) override;
};

