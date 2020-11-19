#include "common.h"

#include "UIOHideable.h"

UIOHideable::UIOHideable(Handle selfHandle, UniqueReference<UIOBase, UIOBase> main_, bool focusOnShow_) {
	this->addElement(std::move(main_));
	this->focusOnShow = focusOnShow_;
}

void UIOHideable::hide() {
	this->hidden = true;
}

void UIOHideable::show() {
	this->hidden = false;
	if (this->focusOnShow) {
		this->shouldFocus = true;
	}
}

CallBackBindResult UIOHideable::runGlobalBinds(State& state) {
	int32_t res = 0;
	if (this->shouldFocus) {
		res |= BIND::RESULT::FOCUS;
		this->shouldFocus = false;
	}

	if (!this->hidden) {
		res |= this->UIOBaseSingle::runGlobalBinds(state);
		if (res & BIND::RESULT::HIDE) {
			res &= ~BIND::RESULT::HIDE;
			this->hidden = true;
		}
		return res;
	}
	else {
		return res | BIND::RESULT::CONTINUE;
	}
}

CallBackBindResult UIOHideable::runFocussedBinds(State& state) {
	if (!this->hidden) {
		int32_t res = this->UIOBaseSingle::runFocussedBinds(state);
		if (res & BIND::RESULT::HIDE) {
			res &= ~BIND::RESULT::HIDE;
			this->hidden = true;
		}
		return res;
	}
	else {
		return BIND::RESULT::CONTINUE;
	}
}

CallBackBindResult UIOHideable::runOnHoverBinds(State& state) {
	if (!this->hidden) {
		int32_t res = this->UIOBaseSingle::runOnHoverBinds(state);
		if (res & BIND::RESULT::HIDE) {
			res &= ~BIND::RESULT::HIDE;
			this->hidden = true;
		}
		return res;
	}
	else {
		return BIND::RESULT::CONTINUE;
	}
}

CallBackBindResult UIOHideable::runActiveBinds(State& state) {
	if (!this->hidden) {
		int32_t res = this->UIOBaseSingle::runActiveBinds(state);
		if (res & BIND::RESULT::HIDE) {
			res &= ~BIND::RESULT::HIDE;
			this->hidden = true;
		}
		return res;
	}
	else {
		return BIND::RESULT::CONTINUE;
	}
}

CallBackBindResult UIOHideable::runGameWorldBinds(State& state) {
	if (!this->hidden) {
		int32_t res = this->UIOBaseSingle::runGameWorldBinds(state);
		if (res & BIND::RESULT::HIDE) {
			res &= ~BIND::RESULT::HIDE;
			this->hidden = true;
		}
		return res;
	}
	else {
		return BIND::RESULT::CONTINUE;
	}
}

int32_t UIOHideable::addRenderInfo(GameState& gameState, RenderInfo& renderInfo, int32_t depth) {
	if (!this->hidden) {
		return this->UIOBaseSingle::addRenderInfo(gameState, renderInfo, depth);
	}
	else {
		return depth;
	}
}
