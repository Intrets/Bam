#pragma once

#include "UIOBase.h"

class UIOShell :public UIOBase
{
public:
	UIOShell(Handle self, UniqueReference<UIOBase, UIOBase> main_);
	UIOShell(Handle self);

	virtual ScreenRectangle updateSize(ScreenRectangle newScreenRectangle) override;
};

