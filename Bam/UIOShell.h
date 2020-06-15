#pragma once

#include "UIOBase.h"

class UIOShell :public UIOBase
{
public:
	UIOBase* main;

	UIOShell(Handle self, UniqueReference<UIOBase, UIOBase> main_);

	virtual ScreenRectangle updateSize(ScreenRectangle newScreenRectangle) override;
};

