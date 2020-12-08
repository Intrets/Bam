#include "common.h"

#include "UIOAnchoredProxy.h"
#include "UIState.h"
#include "UIODestructible.h"
#include "UIOConstructer2.h"
#include "UIOSizeType.h"
#include "UIOCallBackParams.h"
#include "UIOConstrainSize.h"
#include "RenderInfo.h"

void UIOAnchoredProxy::closeProxy() {
	if (this->proxyBase.isValid()) {
		this->destructible->destruct = true;
	}
}

bool UIOAnchoredProxy::hasProxy() {
	return this->proxyBase.isValid();
}

void UIOAnchoredProxy::setProxy(UniqueReference<UIOBase, UIOBase> ref, UIState& uiState) {
	this->closeProxy();

	UIO2::Global::push();

	UIO2::free();
	this->destructible = UIO2::destructible();
	this->proxyBase.set(this->destructible->getSelfHandle());

	auto ptr = UIO2::makeEnd(std::move(ref));
	ptr->addGlobalBind({ CONTROL::KEY::ACTION0 }, [](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
	{
		if (!self_->getScreenRectangle().contains(params.uiState.getCursorPositionScreen())) {
			return BIND::RESULT::CLOSE;
		}
		else {
			return BIND::RESULT::CONTINUE;
		}
	});

	ptr->addOnHoverBind({ CONTROL::KEY::ACTION0 }, [](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
	{
		return BIND::RESULT::CONSUME;
	});

	uiState.addUI(UIO2::Global::pop());
}

ScreenRectangle UIOAnchoredProxy::updateSize(ScreenRectangle newScreenRectangle) {
	this->screenRectangle = newScreenRectangle;
	if (this->proxyBase.isValid()) {
		const float scale = 10.0f;
		switch (this->alignment) {
			case UIO::ALIGNMENT::TOP:
				newScreenRectangle.setBot(-scale);
				break;
			case UIO::ALIGNMENT::BOTTOM:
				newScreenRectangle.setTop(scale);
				break;
			case UIO::ALIGNMENT::LEFT:
				newScreenRectangle.setRight(scale);
				break;
			case UIO::ALIGNMENT::RIGHT:
				newScreenRectangle.setLeft(-scale);
				break;
			case UIO::ALIGNMENT::TOPRIGHT:
				newScreenRectangle.setBottomLeft({ -scale, -scale });
				break;
			case UIO::ALIGNMENT::TOPLEFT:
				newScreenRectangle.setBottomRight({ scale, -scale });
				break;
			case UIO::ALIGNMENT::BOTTOMLEFT:
				newScreenRectangle.setTopRight({ scale, scale });
				break;
			case UIO::ALIGNMENT::BOTTOMRIGHT:
				newScreenRectangle.setTopLeft({ -scale,scale });
				break;
			default:
				break;
		}
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

