#pragma once

#include "UIOBase.h"

class UIOList : public UIOBase
{
public:
	enum class DIR
	{
		LEFT,
		RIGHT,
		UP,
		DOWN,
	};

	DIR direction;

	UIOList(Handle self, DIR dir);

	virtual ScreenRectangle updateSize(ScreenRectangle newScreenRectangle) override;
};

