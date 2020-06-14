#pragma once

#include "UIOBase.h"
#include "UIOSizeType.h"

class UIOPad : public UIOBase
{
public:
	UIOBase* main;

	std::optional<UIOSizeType> top;
	std::optional<UIOSizeType> bottom;
	std::optional<UIOSizeType> left;
	std::optional<UIOSizeType> right;

	UIOPad(Handle self, UniqueReference<UIOBase, UIOBase> main_);

	virtual ScreenRectangle updateSize(ScreenRectangle newScreenRectangle) override;
};

