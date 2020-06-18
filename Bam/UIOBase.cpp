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

void UIOBase::addGlobalBind(BindControl bindControl, CallBack callBack) {
	globalBinds.push_back(std::make_pair(bindControl, callBack));
}

void UIOBase::addFocussedBind(BindControl bindControl, CallBack callBack) {
	focussedBinds.push_back(std::make_pair(bindControl, callBack));
}

CallBackBindResult UIOBase::runGlobalBinds(ControlState& controlState, GameState& gameState) {
	CallBackBindResult sumResult = 0;
	for (auto it = globalBinds.begin(); it != globalBinds.end(); ++it) {
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
		CallBackBindResult elementResult = element.get()->runGlobalBinds(controlState, gameState);
		sumResult |= elementResult;
		if (sumResult & BIND_RESULT::STOP) {
			return sumResult;
		}
	}
	return sumResult;
}

CallBackBindResult UIOBase::runFocussedBinds(ControlState& controlState, GameState& gameState) {
	CallBackBindResult sumResult = 0;
	for (auto it = focussedBinds.begin(); it != focussedBinds.end(); ++it) {
		auto& bind = *it;
		if (controlState.activated(bind.first)) {
			CallBackBindResult bindResult = bind.second(gameState, controlState, this);
			sumResult |= bindResult;
			if (sumResult & BIND_RESULT::STOP) {
				controlState.consumeControl(bind.first.control);
				return sumResult;
			}
		}
		controlState.consumeControl(bind.first.control);
	}
	for (auto& element : elements) {
		CallBackBindResult elementResult = element.get()->runFocussedBinds(controlState, gameState);
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
