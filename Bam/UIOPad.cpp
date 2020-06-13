#include "common.h"

#include "UIOPad.h"

UIOPad::UIOPad(Handle self, UniqueReference<UIOBase, UIOBase> main_) {
	selfHandle = self;
	main = main_.get();
	addElement(std::move(main_));
}

ScreenRectangle UIOPad::updateSize(ScreenRectangle newScreenRectangle) {
	glm::vec2 b;

	static auto getScale = [](std::optional<std::variant<int32_t, float>>& maybeVal, int32_t pixels) -> float {
		float res = 0.0f;
		if (maybeVal) {
			auto val = maybeVal.value();
			if (std::holds_alternative<int32_t>(val)) {
				res = static_cast<float>(std::get<int32_t>(val)) / pixels;
			}
			else {
				res = std::get<float>(val);
			}
		}
		return res;
	};

	glm::vec2 botScale = { 
		getScale(left, screenRectangle.getPixelSize().x), 
		getScale(bottom, screenRectangle.getPixelSize().y) 
	};

	glm::vec2 topScale = { 
		getScale(right, screenRectangle.getPixelSize().x), 
		getScale(top, screenRectangle.getPixelSize().y) 
	};

	newScreenRectangle.setBottomLeft(newScreenRectangle.getBottomLeft() + botScale);
	newScreenRectangle.setTopRight(newScreenRectangle.getTopRight() - topScale);

	screenRectangle = newScreenRectangle;
	main->updateSize(screenRectangle);
	return screenRectangle;
}
