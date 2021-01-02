#include "common.h"

#include "UIOCursor.h"

#include "Inventory.h"
#include "Colors.h"
#include "UIOListSelection.h"
#include "ActivityHelpers.h"
#include "Linker.h"
#include "UIOConstructLuaInterface.h"
#include "UIOConstructer2.h"
#include "UIOSizeType.h"
#include "UIOColoredBackground.h"
#include "UIOWindow.h"
#include "UIOFreeSize.h"
#include "UIOConstructActivityInfo.h"
#include "LUAActivity.h"

ManagedReference<Activity, Activity> const& UIOCursor::getTarget() const {
	return this->target;
}

UIOCursor::UIOCursor(Handle self) {
	this->selfHandle = self;

	this->addGlobalBind({ CONTROL::KEY::MOUSE_POS_CHANGED }, [](PlayerState& playerState, UIOBase* self_) -> CallBackBindResult
	{
		static_cast<UIOCursor*>(self_)->setCursorWorldPosition(playerState.uiState.getCursorPositionWorld());
		static_cast<UIOCursor*>(self_)->setCursorScreenPosition(playerState.uiState.getCursorPositionScreen());

		return BIND::CONTINUE;
	});

	// -----------------------------------
	// Switch rendering in world or in UI
	// -----------------------------------

	this->addGameWorldBind({ CONTROL::KEY::EVERY_TICK }, [](PlayerState& playerState, UIOBase* self_) -> CallBackBindResult
	{
		static_cast<UIOCursor*>(self_)->setWorldRender();
		return BIND::RESULT::CONTINUE;
	});

	// -----------------------------------
	// Cursor rotation
	// -----------------------------------

	this->addGameWorldBind({ CONTROL::KEY::ROTATER, CONTROL::STATE::PRESSED, CONTROL::MODIFIER::SHIFT }, [](PlayerState& playerState, UIOBase* self_) -> CallBackBindResult
	{
		playerState.getPlayer().getInventory().rotateCursorItem(ACTIVITY::ROT::COUNTERCLOCKWISE);
		return BIND::RESULT::CONTINUE;
	});

	this->addGameWorldBind({ CONTROL::KEY::ROTATER, CONTROL::STATE::PRESSED, CONTROL::MODIFIER::NONE }, [](PlayerState& playerState, UIOBase* self_) -> CallBackBindResult
	{
		playerState.getPlayer().getInventory().rotateCursorItem(ACTIVITY::ROT::CLOCKWISE);
		return BIND::RESULT::CONTINUE;
	});

	// -----------------------------------
	// Pick up thing under cursor
	// -----------------------------------

	this->addGameWorldBind({ CONTROL::KEY::ACTION_PICK }, [](PlayerState& playerState, UIOBase* self_) -> CallBackBindResult
	{
		playerState.getPlayer().getInventory().pickupWorld(playerState.gameState, playerState.uiState.getCursorPositionWorld());
		return BIND::RESULT::CONTINUE;
	});

	// -----------------------------------
	// Place/select item
	// -----------------------------------

	this->addGameWorldBind({ CONTROL::KEY::ACTION0 }, [](PlayerState& playerState, UIOBase* self_) -> CallBackBindResult
	{
		static_cast<UIOCursor*>(self_)->clickWorld(playerState);
		return BIND::RESULT::CONTINUE;
	});

	// -----------------------------------
	// Cancel cursor
	// -----------------------------------

	this->addGlobalBind({ CONTROL::KEY::CANCEL }, [](PlayerState& playerState, UIOBase* self_) -> CallBackBindResult
	{
		playerState.getPlayer().getInventory().deselectCursor();
		return BIND::RESULT::CONTINUE;
	});

	UIO2::Global::push();

	this->hoveringFreeElement = UIO2::free();
	this->hoveringElement = UIO2::background(COLORS::UI::RED);
	UIO2::constrainSize({ UIO::SIZETYPE::ABSOLUTE_HEIGHT, 0.1f });
	UIO2::pad({ UIO::SIZETYPE::STATIC_PX, 1 });
	UIO2::background(COLORS::UI::FOREGROUND);
	this->hoveringText = UIO2::textDisplaySingle("testing 123", false);

	this->addElement(UIO2::Global::pop());
}

void UIOCursor::setCursorWorldPosition(glm::vec2 p) {
	this->cursorWorldPosition = p;
}

void UIOCursor::setCursorScreenPosition(glm::vec2 p) {
	this->hoveringElement.get()->translate(p - this->hoveringElement.get()->getScreenRectangle().getBottomLeft());
}

void UIOCursor::clickWorld(PlayerState& playerState) {
	auto pos = playerState.uiState.getCursorPositionWorld();
	auto& gameState = playerState.gameState;

	auto [placed, maybeActivity] = playerState.getPlayer().getInventory().clickWorld(gameState, glm::floor(pos));
	if (maybeActivity.has_value()) {
		auto activity = maybeActivity.value()->getSelfReference();

		if (auto linkTarget = this->target.getRef()) {
			if (!sameGroup(linkTarget, activity)) {
				auto b = Linker::link(gameState, linkTarget, activity);
				(void) b;
				assert(b);
			}
		}
	}
	else {
		if (auto maybeTarget = gameState.staticWorld.getActivity(pos)) {
			this->select(playerState, maybeTarget.value());
		}
		else {
			this->target.unset();
		}
	}
}

void UIOCursor::select(PlayerState& playerState, WeakReference<Activity, Activity> activity) {
	this->target.set(activity);
	this->selectionTick = playerState.gameState.tick;

	if (activity.get()->getType() == ACTIVITY::TYPE::LUA) {
		static int32_t j = 0;
		glm::vec2 offset = glm::vec2(0.05f, -0.05f);
		std::string uiName = "LUA " + std::to_string(activity.getHandle());

		bool newUI =
			playerState.uiState.addNamedUI(
				uiName,
				[activity, uiName, offset]()
		{
			UIO2::Global::push();

			UIO2::window(uiName, { static_cast<float>(j + 1) * offset + glm::vec2(-1.0f, -0.7f), static_cast<float>(j + 1) * offset + glm::vec2(-0.6f, 1.0f) },
						 UIOWindow::TYPE::MINIMISE |
						 UIOWindow::TYPE::RESIZEVERTICAL |
						 UIOWindow::TYPE::RESIZEHORIZONTAL |
						 UIOWindow::TYPE::RESIZE |
						 UIOWindow::TYPE::MOVE |
						 UIOWindow::TYPE::CLOSE);
			UIO2::constructLuaInterface(activity.as<LuaActivity>());

			return UIO2::Global::pop();
		});

		if (newUI) {
			j = (j + 1) % 10;
		}
	}

	using PairType = std::pair<int32_t, ManagedReference<Activity, Activity>>;

	UIO2::Global::push();

	UIO2::window("Activity Info",
				 { 0.65f, -0.9f, 0.95f, 0.0f },
				 UIOWindow::TYPE::MINIMISE |
				 UIOWindow::TYPE::RESIZEVERTICAL |
				 UIOWindow::TYPE::RESIZEHORIZONTAL |
				 UIOWindow::TYPE::RESIZE |
				 UIOWindow::TYPE::MOVE |
				 UIOWindow::TYPE::CLOSE);
	UIO2::constructActivityInfo(this);

	playerState.uiState.addNamedUIReplace(
		"ActivityInfo",
		UIO2::Global::pop()
	);
}

void UIOCursor::setWorldRender() {
	this->renderInWorld = true;
}

int32_t UIOCursor::addRenderInfo(GameState& gameState, RenderInfo& renderInfo, int32_t depth) {
	if (auto player = gameState.getPlayer(renderInfo.playerIndex)) {
		if (auto const& cursorItem = player.value()->getInventory().getCursor()) {
			if (this->renderInWorld) {
				this->renderInWorld = false;
				cursorItem.value().get()->addWorldRenderInfo(gameState, renderInfo, glm::floor(this->cursorWorldPosition));
			}
			this->hoveringText.get()->setText(cursorItem.value().get()->getName());
			this->hoveringElement.get()->addRenderInfo(gameState, renderInfo, 0);
		}

		if (auto targetRef = this->target.getRef()) {
			if (periodic(gameState.tick, 80, 40, -this->selectionTick)) {
				for (auto member : targetRef.get()->getRootPtr()->getTreeMembers()) {
					if (member->getType() != ACTIVITY::TYPE::ANCHOR) {
						member->appendSelectionInfo(gameState, renderInfo, COLORS::GR::SELECTION);
					}
				}
			}
			if (periodic(gameState.tick, 40, 20, -this->selectionTick)) {
				targetRef.get()->appendSelectionInfo(gameState, renderInfo, COLORS::GR::HIGHLIGHT);
			}
		}

	}
	else {
		Locator<Log>::ref().putLine("UIOCursor: did not find player specified in renderInfo");
	}

	return depth;
}

ScreenRectangle UIOCursor::updateSize(ScreenRectangle newScreenRectangle) {
	this->hoveringFreeElement.get()->updateSize(newScreenRectangle);
	return newScreenRectangle;
}
