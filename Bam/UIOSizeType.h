#pragma once

class ScreenRectangle;

struct UIOSizeType
{
	enum TYPE { PX, ABSOLUTE_HEIGHT, ABSOLUTE_WIDTH, RELATIVE_HEIGHT, RELATIVE_WIDTH } type;
	union
	{
		int32_t px;
		float absolute_height;
		float absolute_width;
		float relative_height;
		float relative_width;
	};

	UIOSizeType(UIOSizeType::TYPE t, int32_t val);
	UIOSizeType(UIOSizeType::TYPE t, float val);

	float getWidth(ScreenRectangle screenRectangle);
	float getHeight(ScreenRectangle screenRectangle);
};

