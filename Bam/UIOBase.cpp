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
	translate(p - screenRectangle.getTopLeft());
}

bool UIOBase::contains(glm::vec2 p) {
	return screenRectangle.contains(p);
}

void UIOBase::addBind(BindControl bindControl, CallBack callBack) {
	binds.push_front(std::make_pair(bindControl, callBack));
}

CallBackBindResult UIOBase::runBinds(ControlState& controlState, GameState& gameState) {
	CallBackBindResult sumResult = 0;
	for (auto it = binds.begin(); it != binds.end(); ++it) {
		auto& bind = *it;
		if (controlState.activated(bind.first)) {
			CallBackBindResult bindResult = bind.second(gameState, controlState, this);
			sumResult |= bindResult;
			if (bindResult & BIND_RESULT::CONSUME) {
				controlState.consumeControl(bind.first.control);
			}
			if (sumResult & BIND_RESULT::STOP) {
				return sumResult;
			}
		}
	}
	for (auto& element : elements) {
		CallBackBindResult elementResult = element.get()->runBinds(controlState, gameState);
		sumResult |= elementResult;
		if (sumResult & BIND_RESULT::STOP) {
			return sumResult;
		}
	}
	return sumResult;
}

int32_t UIOBase::addRenderInfo(GameState& gameState, RenderInfo& renderInfo, int32_t depth) {
	int32_t maxDepth = 0;
	for (auto& element : elements) {
		maxDepth = glm::max(maxDepth, element.get()->addRenderInfo(gameState, renderInfo, depth));
	}
	return 1 + maxDepth;
}
