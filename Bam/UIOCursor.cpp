#include "common.h"

#include "UIOCursor.h"
#include "Inventory.h"
#include "UIOCallBackParams.h"
#include "UIOConstructer.h"
#include "UIOTextConstructers.h"
#include "UIOTextDisplay.h"
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

	this->addGameWorldBind({ CONTROL::KEY::ACTION0 }, [](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
	{
		static_cast<UIOCursor*>(self_)->clickWorld(params.gameState, params.uiState.getCursorPositionWorld());
		return BIND::RESULT::CONTINUE;
	});

	this->addGameWorldBind({ CONTROL::KEY::ROTATEL }, [](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
	{
		static_cast<UIOCursor*>(self_)->getInventory().rotateCursorItem(ACTIVITY::ROT::COUNTERCLOCKWISE);
		return BIND::RESULT::CONTINUE;
	});

	this->addGameWorldBind({ CONTROL::KEY::ROTATER }, [](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
	{
		static_cast<UIOCursor*>(self_)->getInventory().rotateCursorItem(ACTIVITY::ROT::CLOCKWISE);
		return BIND::RESULT::CONTINUE;
	});

	this->addElement(
		TextConstructer::constructSingleLineDisplayText("testing 123", false)
		.setPtr(this->hoveringText)
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

void UIOCursor::clickWorld(GameState& gameState, glm::vec2 pos) {
	this->getInventory().clickWorld(gameState, glm::floor(pos));
}

void UIOCursor::setWorldRender() {
	this->renderInWorld = true;
}

int32_t UIOCursor::addRenderInfo(GameState& gameState, RenderInfo& renderInfo, int32_t depth) {
	if (auto const& cursorItem = this->getInventory().getCursor()) {
		if (this->renderInWorld) {
			this->renderInWorld = false;
			cursorItem.value().get()->addWorldRenderInfo(gameState, renderInfo, glm::floor(this->cursorWorldPosition));
		}
		this->hoveringText->setText(cursorItem.value().get()->getName());
		this->hoveringElement->addRenderInfo(gameState, renderInfo, 0);
	}
	return depth;
}

ScreenRectangle UIOCursor::updateSize(ScreenRectangle newScreenRectangle) {
	this->hoveringFreeElement->updateSize(newScreenRectangle);
	return newScreenRectangle;
}
