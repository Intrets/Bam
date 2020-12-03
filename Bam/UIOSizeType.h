#pragma once

class ScreenRectangle;

namespace UIO
{
	enum SIZETYPE
	{
		FH,
		PX,
		STATIC_PX,
		ABSOLUTE_HEIGHT,
		ABSOLUTE_WIDTH,
		RELATIVE_HEIGHT,
		RELATIVE_WIDTH,
	};
}

struct UIOSizeType
{
	UIO::SIZETYPE type;

	union
	{
		int32_t px;
		float relative;
		float absolute_height;
		float absolute_width;
		float relative_height;
		float relative_width;
	};

	UIOSizeType(UIO::SIZETYPE t, int32_t val);
	UIOSizeType(UIO::SIZETYPE t, float val);

	float getWidth(ScreenRectangle screenRectangle) const;
	float getHeight(ScreenRectangle screenRectangle) const;
};

