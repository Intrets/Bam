#include "common.h"

#include "UIOSizeType.h"
#include "ScreenRectangle.h"

UIOSizeType::UIOSizeType(UIOSizeType::TYPE t, int32_t val) {
	type = t;
	assert(t == UIOSizeType::PX);
	px = val;
}

UIOSizeType::UIOSizeType(UIOSizeType::TYPE t, float val) {
	type = t;
	assert(t != UIOSizeType::PX);
	absolute_height = val;
}

float UIOSizeType::getWidth(ScreenRectangle screenRectangle) {
	float widthS;
	auto pixelSize = screenRectangle.getPixelSize();
	switch (type) {
		case UIOSizeType::PX:
			widthS = static_cast<float>(px) / pixelSize.x;
			break;
		case UIOSizeType::ABSOLUTE_HEIGHT:
			widthS = (absolute_height * pixelSize.y) / pixelSize.x;
			break;
		case UIOSizeType::ABSOLUTE_WIDTH:
			widthS = absolute_width;
			break;
		case UIOSizeType::RELATIVE_HEIGHT:
			widthS = (screenRectangle.getHeight() * relative_height * pixelSize.y) / pixelSize.x;
			break;
		case UIOSizeType::RELATIVE_WIDTH:
			widthS = screenRectangle.getWidth() * relative_width;
			break;
		default:
			widthS = screenRectangle.getWidth();
			break;
	}
	return widthS;
}

float UIOSizeType::getHeight(ScreenRectangle screenRectangle) {
	float heightS;
	auto pixelSize = screenRectangle.getPixelSize();
	switch (type) {
		case UIOSizeType::PX:
			heightS = static_cast<float>(px) / pixelSize.y;
			break;
		case UIOSizeType::ABSOLUTE_HEIGHT:
			heightS = absolute_height;
			break;
		case UIOSizeType::ABSOLUTE_WIDTH:
			heightS = (absolute_width * pixelSize.x) / pixelSize.y;
			break;
		case UIOSizeType::RELATIVE_HEIGHT:
			heightS = screenRectangle.getHeight() * relative_height;
			break;
		case UIOSizeType::RELATIVE_WIDTH:
			heightS = (screenRectangle.getWidth() * relative_width * pixelSize.x) / pixelSize.y;
			break;
		default:
			heightS = screenRectangle.getHeight();
			break;
	}
	return heightS;
}
