#pragma once

#include "UIOBase.h"
#include "UIOSizeType.h"
#include "Enums.h"

class UIOPad : public UIOBaseSingle
{
public:
	template<class T>
	friend class UIOConstructer;

	friend class UIOHotbar;

	std::optional<UIOSizeType> top;
	std::optional<UIOSizeType> bottom;
	std::optional<UIOSizeType> left;
	std::optional<UIOSizeType> right;

public:
	UIOPad(Handle self);
	UIOPad(Handle self, UniqueReference<UIOBase, UIOBase> main_);
	UIOPad(Handle self, UniqueReference<UIOBase, UIOBase> main_, UIOSizeType padding);

	virtual ScreenRectangle updateSize(ScreenRectangle newScreenRectangle) override;

	virtual UIO::TYPE getUIOType() override;
};

