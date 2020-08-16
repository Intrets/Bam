#pragma once

#include "UIOBase.h"
#include "ActivityLuaTest.h"

class UIOActivityLuaTest : public UIOBase
{
public:
	ActivityLuaTest lua;

	UIOActivityLuaTest(Handle self);

	virtual ScreenRectangle updateSize(ScreenRectangle newScreenRectangle) override;
};

