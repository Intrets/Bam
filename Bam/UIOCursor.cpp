#include "common.h"

#include "UIOCursor.h"
#include "Inventory.h"
#include "UIOConstructer.h"
#include "UIOTextConstructers.h"
#include "UIOTextDisplay.h"
#include "Colors.h"
#include "UIOListSelection.h"
#include "ActivityHelpers.h"
#include "Linker.h"
#include "UIOConstructLuaInterface.h"
#include "UIOConstructer2.h"

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

	// -----------------------------------
	// Switch rendering in world or in UI
	// -----------------------------------

	this->addGameWorldBind({ CONTROL::KEY::EVERY_TICK }, [](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
	{
		static_cast<UIOCursor*>(self_)->setWorldRender();
		return BIND::RESULT::CONTINUE;
	});

	// -----------------------------------
	// Cursor rotation
	// -----------------------------------

	this->addGameWorldBind({ CONTROL::KEY::ROTATER, CONTROL::STATE::PRESSED, CONTROL::MODIFIER::SHIFT }, [](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
	{
		static_cast<UIOCursor*>(self_)->getInventory().rotateCursorItem(ACTIVITY::ROT::COUNTERCLOCKWISE);
		return BIND::RESULT::CONTINUE;
	});

	this->addGameWorldBind({ CONTROL::KEY::ROTATER, CONTROL::STATE::PRESSED, CONTROL::MODIFIER::NONE }, [](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
	{
		static_cast<UIOCursor*>(self_)->getInventory().rotateCursorItem(ACTIVITY::ROT::CLOCKWISE);
		return BIND::RESULT::CONTINUE;
	});

	// -----------------------------------
	// Pick up thing under cursor
	// -----------------------------------

	this->addGameWorldBind({ CONTROL::KEY::ACTION_PICK }, [](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
	{
		static_cast<UIOCursor*>(self_)->getInventory().pickupWorld(params.gameState, params.uiState.getCursorPositionWorld());
		return BIND::RESULT::CONTINUE;
	});

	// -----------------------------------
	// Place/select item
	// -----------------------------------

	this->addGameWorldBind({ CONTROL::KEY::ACTION0 }, [](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
	{
		static_cast<UIOCursor*>(self_)->clickWorld(params);
		return BIND::RESULT::CONTINUE;
	});

	// -----------------------------------
	// Cancel cursor
	// -----------------------------------

	this->addGlobalBind({ CONTROL::KEY::CANCEL }, [](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
	{
		static_cast<UIOCursor*>(self_)->getInventory().deselectCursor();
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

void UIOCursor::clickWorld(UIOCallBackParams& params) {
	auto pos = params.uiState.getCursorPositionWorld();
	auto& gameState = params.gameState;

	auto [placed, maybeActivity] = this->getInventory().clickWorld(gameState, glm::floor(pos));
	if (maybeActivity.has_value()) {
		WeakReference<Activity, Activity> activity{ maybeActivity.value()->selfHandle };

		if (this->target.isValid()) {
			WeakReference<Activity, Activity> linkTarget = this->target;

			if (sameGroup(linkTarget, activity)) {
				return;
			}
			else {
				Linker::link(gameState, linkTarget, activity);
			}
		}

		this->select(params, activity);
	}
	else {
		if (auto maybeTarget = gameState.staticWorld.getActivity(pos)) {
			this->select(params, maybeTarget.value());
		}
		else {
			this->target.unset();
		}
	}
}

void UIOCursor::select(UIOCallBackParams& params, WeakReference<Activity, Activity> activity) {
	this->target.set(activity);
	this->selectionTick = params.gameState.tick;

	if (activity.get()->getType() == ACTIVITY::TYPE::LUA) {
		static int32_t j = 0;
		glm::vec2 offset = glm::vec2(0.05f, -0.05f);
		std::string uiName = "LUA " + std::to_string(activity.handle);

		bool newUI =
			params.uiState.addNamedUI(
				uiName,
				[activity, uiName, offset]()
		{
			UIO2::Global::start();

			UIO2::window(uiName, { static_cast<float>(j + 1) * offset + glm::vec2(-1.0f, -0.7f), static_cast<float>(j + 1) * offset + glm::vec2(-0.6f, 1.0f) },
						 UIOWindow::TYPE::MINIMISE |
						 UIOWindow::TYPE::RESIZEVERTICAL |
						 UIOWindow::TYPE::RESIZEHORIZONTAL |
						 UIOWindow::TYPE::RESIZE |
						 UIOWindow::TYPE::MOVE |
						 UIOWindow::TYPE::CLOSE);
			UIO2::constructLuaInterface(activity);

			return UIO2::Global::end();
		});

		if (newUI) {
			j = (j + 1) % 10;
		}
	}

	using PairType = std::pair<int32_t, ManagedReference<Activity, Activity>>;

	params.uiState.addNamedUIReplace(
		"ActivityInfo",
		[cursorPtr = this, cursor = ManagedReference<UIOBase, UIOCursor>(this->getSelfHandle())]()
	{
		UIOTextDisplay* labelName;
		UIOListSelection<PairType>* list;

		return UIOConstructer<UIOListSelection<PairType>>::makeConstructer(
			[](PairType const& e)
		{
			if (e.second.isValid()) {
				std::stringstream out;
				out << std::string(e.first, ' ') << "id " << e.second.get()->selfHandle << " type: " << e.second.get()->getTypeName() << " label: " << e.second.get()->getLabel();
				return out.str();
			}
			else {
				return std::string(e.first, ' ') + "invalid";
			}
		})
			.setPtr(list)
			.addBindCapture([cursorPtr](UIOBase* self)
		{
			std::vector<PairType> membersManaged;

			std::vector<std::pair<int32_t, Activity*>> members;
			cursorPtr->target.get()->getRootPtr()->impl_getTreeMembersDepths(members, 0);

			int32_t i = 0;
			int32_t index = 0;
			for (auto [depth, activity] : members) {
				if (activity->selfHandle == cursorPtr->target.getHandle()) {
					index = i;
				}
				membersManaged.push_back({ depth, ManagedReference<Activity, Activity>(activity->selfHandle) });
				i++;
			}

			static_cast<UIOListSelection<PairType>*>(self)->setList(membersManaged);
			static_cast<UIOListSelection<PairType>*>(self)->setSelected(index);
		})
			.list(UIO::DIR::DOWN_REVERSE)
			.addToList(
				TextConstructer::constructSingleLineTextEdit("")
				.setPtr(labelName)
				.constrainWidth({ UIO::SIZETYPE::RELATIVE_WIDTH, 0.5f })
				.list(UIO::DIR::LEFT)
				.addToList(
					TextConstructer::constructSingleLineDisplayText("test")
					.button()
					.onRelease([labelName, list](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
		{
			auto name = labelName->text.getLines()[0];
			name = name.substr(0, name.size() - 1);
			list->getSelected().value()->second.get()->setLabel(name);
			list->invalidateView();
			return BIND::RESULT::CONTINUE;
		})
				)
				.constrainHeight({ UIO::SIZETYPE::FH, 1.2f })
			)
			.window("Activity Info",
					{ 0.65f, -0.9f, 0.95f, 0.0f },
					UIOWindow::TYPE::MINIMISE |
					UIOWindow::TYPE::RESIZEVERTICAL |
					UIOWindow::TYPE::RESIZEHORIZONTAL |
					UIOWindow::TYPE::RESIZE |
					UIOWindow::TYPE::MOVE |
					UIOWindow::TYPE::CLOSE)
			.get();
	});
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

	if (this->target.isValid()) {
		if (periodic(gameState.tick, 80, 40, -this->selectionTick)) {
			for (auto member : this->target.get()->getRootPtr()->getTreeMembers()) {
				if (member->getType() != ACTIVITY::TYPE::ANCHOR) {
					member->appendSelectionInfo(gameState, renderInfo, COLORS::GR::SELECTION);
				}
			}
		}
		if (periodic(gameState.tick, 40, 20, -this->selectionTick)) {
			this->target.get()->appendSelectionInfo(gameState, renderInfo, COLORS::GR::HIGHLIGHT);
		}
	}

	return depth;
}

ScreenRectangle UIOCursor::updateSize(ScreenRectangle newScreenRectangle) {
	this->hoveringFreeElement->updateSize(newScreenRectangle);
	return newScreenRectangle;
}
