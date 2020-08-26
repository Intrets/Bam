#include "common.h"

#include "UIOBase.h"
#include "State.h"

ScreenRectangle const& UIOBase::getScreenRectangle() const {
	return this->screenRectangle;
}

void UIOBase::addElement(UniqueReference<UIOBase, UIOBase> element) {
	this->elements.push_back(std::move(element));
}

void UIOBase::activate() {
	this->active = true;
}

void UIOBase::deactivate() {
	this->active = false;
}

void UIOBase::translate(glm::vec2 p) {
	this->screenRectangle.translate(p);
	//this->updateSize(this->screenRectangle);
	for (auto& element : this->elements) {
		element.get()->translate(p);
	}
}

void UIOBase::setScreenPixels(glm::ivec2 px) {
	this->screenRectangle.setPixelSize(px);
	for (auto& element : this->elements) {
		element.get()->setScreenPixels(px);
	}
}

void UIOBase::moveTopLeftTo(glm::vec2 p) {
	translate(p - this->screenRectangle.getTopLeft());
}

bool UIOBase::contains(glm::vec2 p) const {
	return this->screenRectangle.contains(p);
}

void UIOBase::addGlobalBind(BindControl bindControl, CallBack callBack) {
	this->globalBinds.push_back(std::make_pair(bindControl, callBack));
}

void UIOBase::addFocussedBind(BindControl bindControl, CallBack callBack) {
	this->focussedBinds.push_back(std::make_pair(bindControl, callBack));
}

void UIOBase::addOnHoverBind(BindControl bindControl, CallBack callBack) {
	this->onHoverBinds.push_back(std::make_pair(bindControl, callBack));
}

void UIOBase::addActiveBind(BindControl bindControl, CallBack callBack) {
	this->activeBinds.push_back(std::make_pair(bindControl, callBack));
}

void UIOBase::addGameWorldBind(BindControl bindControl, CallBack callBack) {
	this->gameWorldBinds.push_back(std::make_pair(bindControl, callBack));
}

CallBackBindResult UIOBase::runGlobalBinds(State& state) {
	CallBackBindResult sumResult = 0;

	for (auto& element : this->elements) {
		CallBackBindResult elementResult = element.get()->runGlobalBinds(state);
		sumResult |= elementResult;
		if (sumResult & BIND_RESULT::STOP) {
			return sumResult;
		}
	}

	for (auto [control, bind] : this->globalBinds) {
		if (state.controlState.activated(control)) {
			CallBackBindResult bindResult = bind(state, this);
			sumResult |= bindResult;
			if (bindResult & BIND_RESULT::CONSUME) {
				state.controlState.consumeBufferControl(control.control);
			}
			if (sumResult & BIND_RESULT::STOP) {
				return sumResult;
			}
		}
	}

	return sumResult;
}

CallBackBindResult UIOBase::runFocussedBinds(State& state) {
	CallBackBindResult sumResult = 0;

	for (auto& element : this->elements) {
		CallBackBindResult elementResult = element.get()->runFocussedBinds(state);
		sumResult |= elementResult;
		if (sumResult & BIND_RESULT::STOP) {
			return sumResult;
		}
	}

	for (auto [control, bind] : this->focussedBinds) {
		if (state.controlState.activated(control)) {
			CallBackBindResult bindResult = bind(state, this);
			if (bindResult & BIND_RESULT::CONSUME) {
				state.controlState.consumeBufferControl(control.control);
			}
			sumResult |= bindResult;
			if (sumResult & BIND_RESULT::STOP) {
				return sumResult;
			}
		}
	}

	return sumResult;
}

CallBackBindResult UIOBase::runOnHoverBinds(State& state) {
	CallBackBindResult sumResult = 0;
	if (!this->screenRectangle.contains(state.uiState.getCursorPositionScreen())) {
		return sumResult;
	}

	for (auto& element : this->elements) {
		CallBackBindResult elementResult = element.get()->runOnHoverBinds(state);
		sumResult |= elementResult;
		if (sumResult & BIND_RESULT::STOP) {
			return sumResult;
		}
	}

	for (auto [control, bind] : onHoverBinds) {
		if (state.controlState.activated(control)) {
			CallBackBindResult bindResult = bind(state, this);
			sumResult |= bindResult;
			if (bindResult & BIND_RESULT::CONSUME) {
				state.controlState.consumeBufferControl(control.control);
			}
			if (sumResult & BIND_RESULT::STOP) {
				return sumResult;
			}
		}
	}

	return sumResult;
}

CallBackBindResult UIOBase::runActiveBinds(State& state) {
	CallBackBindResult sumResult = 0;

	for (auto& element : this->elements) {
		CallBackBindResult elementResult = element.get()->runActiveBinds(state);
		sumResult |= elementResult;
		if (sumResult & BIND_RESULT::STOP) {
			return sumResult;
		}
	}

	if (this->active) {
		for (auto [control, bind] : activeBinds) {
			if (state.controlState.activated(control)) {
				CallBackBindResult bindResult = bind(state, this);
				sumResult |= bindResult;
				if (bindResult & BIND_RESULT::CONSUME) {
					state.controlState.consumeBufferControl(control.control);
				}
				if (sumResult & BIND_RESULT::STOP) {
					return sumResult;
				}
			}
		}
	}

	return sumResult;
}

CallBackBindResult UIOBase::runGameWorldBinds(State& state) {
	CallBackBindResult sumResult = 0;

	for (auto& element : this->elements) {
		CallBackBindResult elementResult = element.get()->runGameWorldBinds(state);
		sumResult |= elementResult;
		if (sumResult & BIND_RESULT::STOP) {
			return sumResult;
		}
	}

	for (auto [control, bind] : this->gameWorldBinds) {
		if (state.controlState.activated(control)) {
			CallBackBindResult bindResult = bind(state, this);
			sumResult |= bindResult;
			if (bindResult & BIND_RESULT::CONSUME) {
				state.controlState.consumeBufferControl(control.control);
			}
			if (sumResult & BIND_RESULT::STOP) {
				return sumResult;
			}
		}
	}

	return sumResult;
}

int32_t UIOBase::addRenderInfo(GameState const& gameState, RenderInfo& renderInfo, int32_t depth) {
	int32_t maxDepth = 0;
	for (auto& element : this->elements) {
		maxDepth = glm::max(maxDepth, element.get()->addRenderInfo(gameState, renderInfo, depth));
	}
	return 1 + maxDepth;
}
