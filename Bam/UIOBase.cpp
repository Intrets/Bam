#include "common.h"

#include "UIOBase.h"
#include "State.h"

ScreenRectangle const& UIOBase::getScreenRectangle() const {
	return this->screenRectangle;
}

Handle UIOBase::getSelfHandle() {
	return this->selfHandle;
}

void UIOBaseMulti::addElement(UniqueReference<UIOBase, UIOBase> element) {
	this->elements.push_back(std::move(element));
}

void UIOBase::activate() {
	this->active = true;
}

void UIOBase::deactivate() {
	this->active = false;
}

void UIOBaseMulti::translate(glm::vec2 p) {
	this->screenRectangle.translate(p);
	for (auto& element : this->elements) {
		element.get()->translate(p);
	}
}

void UIOBaseMulti::setScreenPixels(glm::ivec2 px) {
	this->screenRectangle.setPixelSize(px);
	for (auto& element : this->elements) {
		element.get()->setScreenPixels(px);
	}
}

ScreenRectangle UIOBaseMulti::updateSize(ScreenRectangle newScreenRectangle) {
	this->screenRectangle = newScreenRectangle;

	for (auto& element : this->elements) {
		element.get()->updateSize(this->screenRectangle);
	}

	return this->screenRectangle;
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

	for (auto [control, bind] : this->globalBinds) {
		if (state.controlState.activated(control)) {
			CallBackBindResult bindResult = bind(state, this);
			sumResult |= bindResult;
			if (bindResult & BIND::RESULT::CONSUME) {
				state.controlState.consumeBufferControl(control.control);
			}
			if (sumResult & BIND::RESULT::STOP) {
				return sumResult;
			}
		}
	}

	return sumResult;
}

CallBackBindResult UIOBase::runFocussedBinds(State& state) {
	CallBackBindResult sumResult = 0;

	for (auto [control, bind] : this->focussedBinds) {
		if (state.controlState.activated(control)) {
			CallBackBindResult bindResult = bind(state, this);
			if (bindResult & BIND::RESULT::CONSUME) {
				state.controlState.consumeBufferControl(control.control);
			}
			sumResult |= bindResult;
			if (sumResult & BIND::RESULT::STOP) {
				return sumResult;
			}
		}
	}

	return sumResult;
}

CallBackBindResult UIOBase::runOnHoverBinds(State& state) {
	CallBackBindResult sumResult = 0;

	for (auto [control, bind] : onHoverBinds) {
		if (state.controlState.activated(control)) {
			CallBackBindResult bindResult = bind(state, this);
			sumResult |= bindResult;
			if (bindResult & BIND::RESULT::CONSUME) {
				state.controlState.consumeBufferControl(control.control);
			}
			if (sumResult & BIND::RESULT::STOP) {
				return sumResult;
			}
		}
	}

	return sumResult;
}

CallBackBindResult UIOBase::runActiveBinds(State& state) {
	CallBackBindResult sumResult = 0;

	if (this->active) {
		for (auto [control, bind] : activeBinds) {
			if (state.controlState.activated(control)) {
				CallBackBindResult bindResult = bind(state, this);
				sumResult |= bindResult;
				if (bindResult & BIND::RESULT::CONSUME) {
					state.controlState.consumeBufferControl(control.control);
				}
				if (sumResult & BIND::RESULT::STOP) {
					return sumResult;
				}
			}
		}
	}

	return sumResult;
}

CallBackBindResult UIOBase::runGameWorldBinds(State& state) {
	CallBackBindResult sumResult = 0;

	for (auto [control, bind] : this->gameWorldBinds) {
		if (state.controlState.activated(control)) {
			CallBackBindResult bindResult = bind(state, this);
			sumResult |= bindResult;
			if (bindResult & BIND::RESULT::CONSUME) {
				state.controlState.consumeBufferControl(control.control);
			}
			if (sumResult & BIND::RESULT::STOP) {
				return sumResult;
			}
		}
	}

	return sumResult;
}

CallBackBindResult UIOBaseMulti::runGlobalBinds(State& state) {
	CallBackBindResult sumResult = 0;

	for (auto& element : this->elements) {
		CallBackBindResult elementResult = element.get()->runGlobalBinds(state);
		sumResult |= elementResult;
		if (sumResult & BIND::RESULT::STOP) {
			return sumResult;
		}
	}

	return sumResult | this->UIOBase::runGlobalBinds(state);
}

CallBackBindResult UIOBaseMulti::runFocussedBinds(State& state) {
	CallBackBindResult sumResult = 0;

	for (auto& element : this->elements) {
		CallBackBindResult elementResult = element.get()->runFocussedBinds(state);
		sumResult |= elementResult;
		if (sumResult & BIND::RESULT::STOP) {
			return sumResult;
		}
	}

	return sumResult | this->UIOBase::runFocussedBinds(state);
}

CallBackBindResult UIOBaseMulti::runOnHoverBinds(State& state) {
	CallBackBindResult sumResult = 0;
	if (!this->screenRectangle.contains(state.uiState.getCursorPositionScreen())) {
		return sumResult;
	}

	for (auto& element : this->elements) {
		CallBackBindResult elementResult = element.get()->runOnHoverBinds(state);
		sumResult |= elementResult;
		if (sumResult & BIND::RESULT::STOP) {
			return sumResult;
		}
	}

	return sumResult | this->UIOBase::runOnHoverBinds(state);
}

CallBackBindResult UIOBaseMulti::runActiveBinds(State& state) {
	CallBackBindResult sumResult = 0;

	for (auto& element : this->elements) {
		CallBackBindResult elementResult = element.get()->runActiveBinds(state);
		sumResult |= elementResult;
		if (sumResult & BIND::RESULT::STOP) {
			return sumResult;
		}
	}

	return sumResult | this->UIOBase::runActiveBinds(state);
}

CallBackBindResult UIOBaseMulti::runGameWorldBinds(State& state) {
	CallBackBindResult sumResult = 0;

	for (auto& element : this->elements) {
		CallBackBindResult elementResult = element.get()->runGameWorldBinds(state);
		sumResult |= elementResult;
		if (sumResult & BIND::RESULT::STOP) {
			return sumResult;
		}
	}

	return sumResult | this->UIOBase::runGameWorldBinds(state);
}

int32_t UIOBaseMulti::addRenderInfo(GameState& gameState, RenderInfo& renderInfo, int32_t depth) {
	int32_t maxDepth = 0;
	for (auto& element : this->elements) {
		maxDepth = glm::max(maxDepth, element.get()->addRenderInfo(gameState, renderInfo, depth));
	}
	return 1 + maxDepth;
}

void UIOBaseSingle::addElement(UniqueReference<UIOBase, UIOBase> element) {
	//assert(main.isNull());
	this->main = std::move(element);
}

void UIOBaseSingle::translate(glm::vec2 p) {
	//assert(this->main.isNotNull());
	this->screenRectangle.translate(p);
	this->main.get()->translate(p);
}

void UIOBaseSingle::setScreenPixels(glm::ivec2 px) {
	//assert(this->main.isNotNull());
	this->screenRectangle.setPixelSize(px);
	this->main.get()->setScreenPixels(px);
}

ScreenRectangle UIOBaseSingle::updateSize(ScreenRectangle newScreenRectangle) {
	this->screenRectangle = this->main.get()->updateSize(newScreenRectangle);
	return this->screenRectangle;
}

CallBackBindResult UIOBaseSingle::runGlobalBinds(State& state) {
	CallBackBindResult sumResult = this->main.get()->runGlobalBinds(state);
	if (sumResult & BIND::RESULT::STOP) {
		return sumResult;
	}

	return sumResult | this->UIOBase::runGlobalBinds(state);
}

CallBackBindResult UIOBaseSingle::runFocussedBinds(State& state) {
	CallBackBindResult sumResult = this->main.get()->runFocussedBinds(state);
	if (sumResult & BIND::RESULT::STOP) {
		return sumResult;
	}

	return sumResult | this->UIOBase::runFocussedBinds(state);
}

CallBackBindResult UIOBaseSingle::runOnHoverBinds(State& state) {
	CallBackBindResult sumResult = 0;
	if (!this->screenRectangle.contains(state.uiState.getCursorPositionScreen())) {
		return sumResult;
	}

	sumResult = this->main.get()->runOnHoverBinds(state);

	if (sumResult & BIND::RESULT::STOP) {
		return sumResult;
	}

	return sumResult | this->UIOBase::runOnHoverBinds(state);
}

CallBackBindResult UIOBaseSingle::runActiveBinds(State& state) {
	CallBackBindResult sumResult = this->main.get()->runActiveBinds(state);
	if (sumResult & BIND::RESULT::STOP) {
		return sumResult;
	}

	return sumResult | this->UIOBase::runActiveBinds(state);
}

CallBackBindResult UIOBaseSingle::runGameWorldBinds(State& state) {
	CallBackBindResult sumResult = this->main.get()->runGameWorldBinds(state);
	if (sumResult & BIND::RESULT::STOP) {
		return sumResult;
	}

	return sumResult | this->UIOBase::runGameWorldBinds(state);
}

int32_t UIOBaseSingle::addRenderInfo(GameState& gameState, RenderInfo& renderInfo, int32_t depth) {
	assert(main.isNotNull());
	return this->main.get()->addRenderInfo(gameState, renderInfo, depth);
}

void UIOBaseEnd::addElement(UniqueReference<UIOBase, UIOBase> element) {
	assert(0);
}

void UIOBaseEnd::translate(glm::vec2 p) {
	this->screenRectangle.translate(p);
}

void UIOBaseEnd::setScreenPixels(glm::ivec2 px) {
	this->screenRectangle.setPixelSize(px);
}
