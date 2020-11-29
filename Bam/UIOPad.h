#pragma once

#include "UIOBase.h"
#include "UIOSizeType.h"
#include "Enums.h"

class UIOPad : public UIOBaseSingle
{
public:
	friend class UIOHotbar;

	std::optional<UIOSizeType> top;
	std::optional<UIOSizeType> bottom;
	std::optional<UIOSizeType> left;
	std::optional<UIOSizeType> right;

public:
	UIOPad(Handle self);
	virtual ~UIOPad() = default;

	virtual ScreenRectangle updateSize(ScreenRectangle newScreenRectangle) override;

	virtual UIO::TYPE getUIOType() override;
};

