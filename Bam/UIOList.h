#pragma once

#include "UIOBase.h"

class UIOList : public UIOBase
{
public:
	enum class DIRECTION
	{
		LEFT,
		RIGHT,
		UP,
		DOWN,
	};

	DIRECTION direction;

	UIOList(Handle self, DIRECTION dir);

	virtual ScreenRectangle updateSize(ScreenRectangle newScreenRectangle) override;
};

