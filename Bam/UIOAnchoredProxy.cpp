#include "common.h"

#include "UIOAnchoredProxy.h"
#include "UIState.h"
#include "UIODestructible.h"
#include "UIOConstructer2.h"
#include "UIOSizeType.h"
#include "UIOCallBackParams.h"
#include "UIOConstrainSize.h"
#include "RenderInfo.h"

void UIOAnchoredProxy::setProxy(UniqueReference<UIOBase, UIOBase> ref, UIState& uiState) {
	if (this->proxyBase.isValid()) {
		this->destructible->destruct = true;
	}

	UIO2::Global::push();


	UIO2::free();
	this->destructible = UIO2::destructible();
	this->proxyBase.set(this->destructible->getSelfHandle());

	auto ptr = UIO2::makeEnd(std::move(ref));
	ptr->addGlobalBind({ CONTROL::KEY::ANYTHING_KEY }, [](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
	{
		if (!self_->getScreenRectangle().contains(params.uiState.getCursorPositionScreen())) {
			return BIND::RESULT::CLOSE;
		}
		else {
			return BIND::RESULT::CONTINUE;
		}
	});

	uiState.addUI(UIO2::Global::pop());
}

ScreenRectangle UIOAnchoredProxy::updateSize(ScreenRectangle newScreenRectangle) {
	this->screenRectangle = newScreenRectangle;
	if (this->proxyBase.isValid()) {
		this->proxyBase.get()->updateSize(newScreenRectangle);
	}
	return this->screenRectangle;
}

int32_t UIOAnchoredProxy::addRenderInfo(GameState& gameState, RenderInfo& renderInfo, int32_t depth) {
	return depth;
}

UIOAnchoredProxy::UIOAnchoredProxy(Handle self) {
	this->selfHandle = self;
}

UIOAnchoredProxy::~UIOAnchoredProxy() {
	if (this->proxyBase.isValid()) {
		this->destructible->destruct = true;
	}
}

