#include "common.h"

#include "UIOHideable.h"

UIOHideable::UIOHideable(Handle self) {
	this->selfHandle = self;
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

CallBackBindResult UIOHideable::runGlobalBinds(PlayerState& playerState) {
	int32_t res = 0;
	if (this->shouldFocus) {
		res |= BIND::RESULT::FOCUS;
		this->shouldFocus = false;
	}

	if (!this->hidden) {
		res |= this->UIOBaseSingle::runGlobalBinds(playerState);
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

CallBackBindResult UIOHideable::runFocussedBinds(PlayerState& playerState) {
	if (!this->hidden) {
		int32_t res = this->UIOBaseSingle::runFocussedBinds(playerState);
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

CallBackBindResult UIOHideable::runOnHoverBinds(PlayerState& playerState) {
	if (!this->hidden) {
		int32_t res = this->UIOBaseSingle::runOnHoverBinds(playerState);
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

CallBackBindResult UIOHideable::runActiveBinds(PlayerState& playerState) {
	if (!this->hidden) {
		int32_t res = this->UIOBaseSingle::runActiveBinds(playerState);
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

CallBackBindResult UIOHideable::runGameWorldBinds(PlayerState& playerState) {
	if (!this->hidden) {
		int32_t res = this->UIOBaseSingle::runGameWorldBinds(playerState);
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
