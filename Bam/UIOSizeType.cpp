#include "common.h"

#include "UIOSizeType.h"
#include "Rectangle.h"
#include "Option.h"

UIOSizeType::UIOSizeType(UIOSizeType::TYPE t, int32_t val) {
	this->type = t;
	assert(t == UIOSizeType::STATIC_PX || t == UIOSizeType::PX);
	this->px = val;
}

UIOSizeType::UIOSizeType(UIOSizeType::TYPE t, float val) {
	this->type = t;
	assert(t != UIOSizeType::STATIC_PX);
	this->absolute_height = val;
}

float UIOSizeType::getWidth(ScreenRectangle screenRectangle) {
	float widthS;
	auto pixelSize = screenRectangle.getPixelSize();
	switch (this->type) {
		case UIOSizeType::PX:
			widthS = Option<OPTION::UI_SCALE, float>::getVal() * 2.0f * static_cast<float>(this->px) / pixelSize.x;
			break;
		case UIOSizeType::FH:
			widthS = Option<OPTION::UI_SCALE, float>::getVal() * this->relative * 2.0f * 17.0f / pixelSize.x;
			break;
		case UIOSizeType::STATIC_PX:
			widthS = 2 * static_cast<float>(this->px) / pixelSize.x;
			break;
		case UIOSizeType::ABSOLUTE_HEIGHT:
			widthS = (this->absolute_height * pixelSize.y) / pixelSize.x;
			break;
		case UIOSizeType::ABSOLUTE_WIDTH:
			widthS = this->absolute_width;
			break;
		case UIOSizeType::RELATIVE_HEIGHT:
			widthS = (screenRectangle.getHeight() * this->relative_height * pixelSize.y) / pixelSize.x;
			break;
		case UIOSizeType::RELATIVE_WIDTH:
			widthS = screenRectangle.getWidth() * this->relative_width;
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
			heightS = Option<OPTION::UI_SCALE, float>::getVal() * 2 * static_cast<float>(this->px) / pixelSize.y;
			break;
		case UIOSizeType::FH:
			heightS = Option<OPTION::UI_SCALE, float>::getVal() * this->relative * 2.0f * 17.0f / pixelSize.y;
			break;
		case UIOSizeType::STATIC_PX:
			heightS = 2 * static_cast<float>(this->px) / pixelSize.y;
			break;
		case UIOSizeType::ABSOLUTE_HEIGHT:
			heightS = this->absolute_height;
			break;
		case UIOSizeType::ABSOLUTE_WIDTH:
			heightS = (this->absolute_width * pixelSize.x) / pixelSize.y;
			break;
		case UIOSizeType::RELATIVE_HEIGHT:
			heightS = screenRectangle.getHeight() * this->relative_height;
			break;
		case UIOSizeType::RELATIVE_WIDTH:
			heightS = (screenRectangle.getWidth() * this->relative_width * pixelSize.x) / pixelSize.y;
			break;
		default:
			heightS = screenRectangle.getHeight();
			break;
	}
	return heightS;
}
