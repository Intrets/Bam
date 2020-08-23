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

private:
	DIR direction;

public:
	UIOList(Handle self, DIR dir);

	virtual ScreenRectangle updateSize(ScreenRectangle newScreenRectangle) override;
};

