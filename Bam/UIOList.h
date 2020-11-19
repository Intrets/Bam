#pragma once

#include "UIOBase.h"

namespace UIO
{
	enum class DIR
	{
		LEFT,
		RIGHT,
		UP,
		DOWN,
		LEFT_REVERSE,
		RIGHT_REVERSE,
		UP_REVERSE,
		DOWN_REVERSE,
	};
}

class UIOList : public UIOBaseMulti
{
private:
	UIO::DIR direction;

public:
	UIOList(Handle self, UIO::DIR dir);

	virtual ScreenRectangle updateSize(ScreenRectangle newScreenRectangle) override;
};

