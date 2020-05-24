#include "common.h"

#include "UIOBase.h"

void UIOBase::addElement(UniqueReference<UIOBase, UIOBase> element) {
	elements.push_back(std::move(element));
}

void UIOBase::translate(glm::vec2 p) {
	screenRectangle.translate(p);
	for (auto& element : elements) {
		element.get()->translate(p);
	}
}

void UIOBase::setScreenPixels(glm::ivec2 px) {
	screenRectangle.screenPixels = px;
	for (auto& element : elements) {
		element.get()->setScreenPixels(px);
	}
}

void UIOBase::moveTopLeftTo(glm::vec2 p) {
	translate(p - screenRectangle.topLeft());
}

void UIOBase::scale(glm::vec2 s) {
	screenRectangle.scaleAnchorTopLeft(s);
	updateSize(screenRectangle);
}

bool UIOBase::contains(glm::vec2 p) {
	return screenRectangle.contains(p);
}

void UIOBase::addBind(BindControl bindControl, CallBack callBack) {
	binds.push_front(std::make_pair(bindControl, callBack));
}

void UIOBase::runBinds(ControlState& controlState, GameState& gameState) {
	for (auto it = binds.begin(); it != binds.end();) {
		auto& bind = *it;
		if (controlState.activated(bind.first) && bind.second(gameState, controlState, this)) {
			it = binds.erase(it);
		}
		else {
			++it;
		}
	}
	for (auto& element : elements) {
		element.get()->runBinds(controlState, gameState);
	}
}

int32_t UIOBase::addRenderInfo(RenderInfo& renderInfo, int32_t depth) {
	int32_t maxDepth = 0;
	for (auto& element : elements) {
		maxDepth = glm::max(maxDepth, element.get()->addRenderInfo(renderInfo, depth));
	}
	return maxDepth;
}
