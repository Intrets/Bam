#include "common.h"

#include "UIOHideable.h"

UIOHideable::UIOHideable(Handle selfHandle, UniqueReference<UIOBase, UIOBase> main_, bool focusOnShow_) {
	this->main = main_.get();
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
		res |= this->UIOBase::runGlobalBinds(state);
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
		int32_t res = this->UIOBase::runFocussedBinds(state);
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
		int32_t res = this->UIOBase::runOnHoverBinds(state);
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
		int32_t res = this->UIOBase::runActiveBinds(state);
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
		int32_t res = this->UIOBase::runGameWorldBinds(state);
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
		return this->UIOBase::addRenderInfo(gameState, renderInfo, depth);
	}
	else {
		return depth;
	}
}

ScreenRectangle UIOHideable::updateSize(ScreenRectangle newScreenRectangle) {
	this->screenRectangle = this->main->updateSize(newScreenRectangle);
	return this->screenRectangle;
}
