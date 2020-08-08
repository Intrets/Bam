#include "common.h"

#include "UIOBase.h"
#include "State.h"

void UIOBase::addElement(UniqueReference<UIOBase, UIOBase> element) {
	this->elements.push_back(std::move(element));
}

void UIOBase::translate(glm::vec2 p) {
	this->screenRectangle.translate(p);
	//this->updateSize(this->screenRectangle);
	for (auto& element : this->elements) {
		element.get()->translate(p);
	}
}

void UIOBase::setScreenPixels(glm::ivec2 px) {
	this->screenRectangle.screenPixels = px;
	for (auto& element : this->elements) {
		element.get()->setScreenPixels(px);
	}
}

void UIOBase::moveTopLeftTo(glm::vec2 p) {
	translate(p - this->screenRectangle.getTopLeft());
}

bool UIOBase::contains(glm::vec2 p) {
	return this->screenRectangle.contains(p);
}

void UIOBase::addGlobalBind(BindControl bindControl, CallBack callBack) {
	globalBinds.push_back(std::make_pair(bindControl, callBack));
}

void UIOBase::addFocussedBind(BindControl bindControl, CallBack callBack) {
	focussedBinds.push_back(std::make_pair(bindControl, callBack));
}

void UIOBase::addClickBind(BindControl bindControl, CallBack callBack) {
}

CallBackBindResult UIOBase::runGlobalBinds(State& state) {
	CallBackBindResult sumResult = 0;
	for (auto [control, bind] : this->globalBinds) {
		if (state.controlState.activated(control)) {
			CallBackBindResult bindResult = bind(state, this);
			sumResult |= bindResult;
			if (bindResult & BIND_RESULT::CONSUME) {
				state.controlState.consumeControl(control.control);
			}
			if (sumResult & BIND_RESULT::STOP) {
				return sumResult;
			}
		}
	}
	for (auto& element : this->elements) {
		CallBackBindResult elementResult = element.get()->runGlobalBinds(state);
		sumResult |= elementResult;
		if (sumResult & BIND_RESULT::STOP) {
			return sumResult;
		}
	}
	return sumResult;
}

CallBackBindResult UIOBase::runFocussedBinds(State& state) {
	CallBackBindResult sumResult = 0;
	for (auto [control, bind] : this->focussedBinds) {
		if (state.controlState.activated(control)) {
			CallBackBindResult bindResult = bind(state, this);
			if (bindResult & BIND_RESULT::CONSUME) {
				state.controlState.consumeControl(control.control);
			}
			sumResult |= bindResult;
			if (sumResult & BIND_RESULT::STOP) {
				return sumResult;
			}
		}
	}
	for (auto& element : this->elements) {
		CallBackBindResult elementResult = element.get()->runFocussedBinds(state);
		sumResult |= elementResult;
		if (sumResult & BIND_RESULT::STOP) {
			return sumResult;
		}
	}
	return sumResult;
}

CallBackBindResult UIOBase::runClickBinds(State& state) {
	CallBackBindResult sumResult = 0;
	if (!this->screenRectangle.contains(state.uiState.getCursorPositionScreen())) {
		for (auto [control, bind] : clickBinds) {
			if (state.controlState.activated(control)) {
				CallBackBindResult bindResult = bind(state, this);
				sumResult |= bindResult;
				if (bindResult & BIND_RESULT::CONSUME) {
					state.controlState.consumeControl(control.control);
				}
				if (sumResult & BIND_RESULT::STOP) {
					return sumResult;
				}
			}
		}
	}
	return CallBackBindResult();
}

int32_t UIOBase::addRenderInfo(GameState& gameState, RenderInfo& renderInfo, int32_t depth) {
	int32_t maxDepth = 0;
	for (auto& element : this->elements) {
		maxDepth = glm::max(maxDepth, element.get()->addRenderInfo(gameState, renderInfo, depth));
	}
	return 1 + maxDepth;
}
