#pragma once

#include "UIOBase.h"

class UIOList : public UIOBase
{
public:
	enum class DIRECTION
	{
		VERTICAL,
		HORIZONTAL,
	};

	DIRECTION direction;

	UIOList(Handle self, DIRECTION dir);

	virtual ScreenRectangle updateSize(ScreenRectangle newScreenRectangle) override;
};

