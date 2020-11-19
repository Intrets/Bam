#include "common.h"

#include "UIOEmpty.h"

UIOEmpty::UIOEmpty(Handle self_) {
	this->selfHandle = self_;
}

ScreenRectangle UIOEmpty::updateSize(ScreenRectangle newScreenRectangle) {
	return newScreenRectangle;
}

void UIOEmpty::addElement(UniqueReference<UIOBase, UIOBase> element) {
}

void UIOEmpty::translate(glm::vec2 p) {
}

void UIOEmpty::setScreenPixels(glm::ivec2 px) {
}

int32_t UIOEmpty::addRenderInfo(GameState& gameState, RenderInfo& renderInfo, int32_t depth) {
	return depth;
}
