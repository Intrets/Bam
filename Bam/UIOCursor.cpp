#include "common.h"

#include "UIOCursor.h"
#include "Inventory.h"
#include "UIOCallBackParams.h"
#include "UIOConstructer.h"
#include "UIOTextConstructers.h"
#include "Colors.h"

Inventory& UIOCursor::getInventory() {
	return Locator<Inventory>::ref();
}

UIOCursor::UIOCursor(Handle self) {
	this->selfHandle = self;

	this->addGlobalBind({ CONTROL::KEY::MOUSE_POS_CHANGED }, [](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
	{
		static_cast<UIOCursor*>(self_)->setCursorWorldPosition(params.uiState.getCursorPositionWorld());
		static_cast<UIOCursor*>(self_)->setCursorScreenPosition(params.uiState.getCursorPositionScreen());

		return BIND::CONTINUE;
	});

	this->addGameWorldBind({ CONTROL::KEY::EVERY_TICK }, [](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
	{
		static_cast<UIOCursor*>(self_)->setWorldRender();
		return BIND::RESULT::CONTINUE;
	});

	this->addElement(
		TextConstructer::constructSingleLineDisplayText("testing 123", false)
		.background(COLORS::UI::FOREGROUND)
		.pad({ UIO::SIZETYPE::STATIC_PX, 1 })
		.constrainHeight({ UIO::SIZETYPE::ABSOLUTE_HEIGHT, 0.1f })
		.constrainWidth({ UIO::SIZETYPE::ABSOLUTE_HEIGHT, 0.1f })
		.background(COLORS::UI::RED)
		.setPtr(this->hoveringElement)
		.free()
		.setPtr(this->hoveringFreeElement)
		.get()
	);
}

void UIOCursor::setCursorWorldPosition(glm::vec2 p) {
	this->cursorWorldPosition = p;
}

void UIOCursor::setCursorScreenPosition(glm::vec2 p) {
	this->hoveringElement->translate(p - this->hoveringElement->getScreenRectangle().getBottomLeft());
}

void UIOCursor::setWorldRender() {
	this->renderInWorld = true;
}

int32_t UIOCursor::addRenderInfo(GameState& gameState, RenderInfo& renderInfo, int32_t depth) {
	if (auto const& cursorItem = this->getInventory().getCursor()) {
		if (this->renderInWorld) {
			this->renderInWorld = false;
			auto const& item = cursorItem.value();

			item.get()->addWorldRenderInfo(gameState, renderInfo, glm::floor(this->cursorWorldPosition));
		}
		else {
			this->hoveringElement->addRenderInfo(gameState, renderInfo, 0);
		}
	}
	return depth;
}

ScreenRectangle UIOCursor::updateSize(ScreenRectangle newScreenRectangle) {
	this->hoveringFreeElement->updateSize(newScreenRectangle);
	return newScreenRectangle;
}
