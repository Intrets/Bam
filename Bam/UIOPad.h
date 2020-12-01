#pragma once

#include "UIOBase.h"
#include "UIOSizeType.h"
#include "Enums.h"

struct UIOPadType
{
	UIOPAD::TYPE type;

	UIOSizeType size;

	UIOSizeType const& getSizeType() const;
	UIOPadType& operator=(UIOPAD::TYPE const& t);
	UIOPadType& operator=(UIOSizeType const& s);

	UIOPadType();
	UIOPadType(UIOPAD::TYPE t);
	UIOPadType(UIOSizeType s);
};

class UIOPad : public UIOBaseSingle
{
public:
	friend class UIOHotbar;

	UIOPadType top;
	UIOPadType bottom;
	UIOPadType left;
	UIOPadType right;

public:
	UIOPad(Handle self);
	virtual ~UIOPad() = default;

	virtual ScreenRectangle updateSize(ScreenRectangle newScreenRectangle) override;

	virtual UIO::TYPE getUIOType() override;
};

