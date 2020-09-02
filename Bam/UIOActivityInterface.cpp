#include "common.h"

#include "UIOActivityInterface.h"

#include "Colors.h"
#include "GameState.h"
#include "Platform.h"
#include "Piston.h"
#include "UIOCallBackParams.h"
#include "Linker.h"
#include "ActivityHelpers.h"
#include "UIOCallBackParams.h"
#include "Anchor.h"
#include "ActivitySpawner.h"
#include "UIOConstructLuaInterface.h"
#include "UIOList.h"
#include "UIOConstructer.h"

UIOActivityInterface::UIOActivityInterface(Handle self) {
	this->selfHandle = self;

	this->addFocussedBind({ CONTROL::KEY::CANCEL }, [](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
	{
		auto self = static_cast<UIOActivityInterface*>(self_);
		self->exit();
		return BIND::RESULT::CONTINUE | BIND::RESULT::HIDE;
	});

	this->addFocussedBind({ CONTROL::KEY::ROTATER, CONTROL::STATE::PRESSED, CONTROL::MODIFIER::NONE }, [](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
	{
		auto self = static_cast<UIOActivityInterface*>(self_);
		self->rotateHover(ACTIVITY::ROT::CLOCKWISE);
		return BIND::RESULT::CONTINUE | BIND::RESULT::CONSUME;
	});

	this->addFocussedBind({ CONTROL::KEY::ROTATER, CONTROL::STATE::PRESSED, CONTROL::MODIFIER::SHIFT }, [](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
	{
		auto self = static_cast<UIOActivityInterface*>(self_);
		self->rotateHover(ACTIVITY::ROT::COUNTERCLOCKWISE);
		return BIND::RESULT::CONTINUE | BIND::RESULT::CONSUME;
	});

	this->addGameWorldBind({ CONTROL::KEY::ACTION1 }, [](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
	{
		auto self = static_cast<UIOActivityInterface*>(self_);
		self->cancel(false);
		return BIND::RESULT::CONTINUE | BIND::RESULT::CONSUME;
	});

	this->addGameWorldBind({ CONTROL::KEY::ACTION_USE }, [](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
	{
		auto self = static_cast<UIOActivityInterface*>(self_);
		self->pickUp(params.gameState, params.uiState.getCursorPositionWorld());
		return BIND::RESULT::CONTINUE | BIND::RESULT::CONSUME | BIND::RESULT::FOCUS;
	});

	this->addGameWorldBind({ CONTROL::KEY::ACTION_ACTIVATE, CONTROL::STATE::PRESSED | CONTROL::STATE::REPEAT, CONTROL::MODIFIER::NONE }, [](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
	{
		auto self = static_cast<UIOActivityInterface*>(self_);
		self->changeHoverActivityState(2);
		return BIND::RESULT::CONTINUE | BIND::RESULT::CONSUME;
	});

	this->addGameWorldBind({ CONTROL::KEY::ACTION_ACTIVATE, CONTROL::STATE::PRESSED | CONTROL::STATE::REPEAT, CONTROL::MODIFIER::SHIFT }, [](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
	{
		auto self = static_cast<UIOActivityInterface*>(self_);
		self->changeHoverActivityState(1);
		return BIND::RESULT::CONTINUE | BIND::RESULT::CONSUME;
	});

	this->addFocussedBind({ CONTROL::KEY::MOUSE_POS_CHANGED }, [](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
	{
		auto self = static_cast<UIOActivityInterface*>(self_);
		self->updateCursorPos(params.uiState.getCursorPositionWorld());
		return BIND::RESULT::CONTINUE;
	});

	this->addFocussedBind({ CONTROL::KEY::ACTION_DELETE }, [](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
	{
		auto self = static_cast<UIOActivityInterface*>(self_);
		self->splitTarget();
		return BIND::RESULT::CONTINUE | BIND::RESULT::CONSUME;
	});
}

void UIOActivityInterface::exit() {
	this->cursor.deleteObject();
	this->target.unset();
	this->base.unset();
	this->type = UIO::USER_ACTION_TYPE::NOTHING;
}

void UIOActivityInterface::cancel(bool full) {
	if (full) {
		this->type = UIO::USER_ACTION_TYPE::NOTHING;
		this->cursor.deleteObject();
		this->target.unset();
		this->base.unset();
	}
	else {
		switch (this->type) {
			case UIO::USER_ACTION_TYPE::HOVERING:
				if (this->cursor.isNotNull()) {
					this->cursor.deleteObject();
				}
				this->type = UIO::USER_ACTION_TYPE::NOTHING;
				break;
			case UIO::USER_ACTION_TYPE::NOTHING:
				if (this->target.isValid()) {
					this->target.unset();
				}
				else if (this->base.isValid()) {
					this->base.unset();
				}
				break;
			default:
				break;
		}
	}
}

void UIOActivityInterface::setBase(WeakReference<Activity, Activity> ref) {
	this->base.set(ref);
	this->target.unset();
	this->baseSelectionTick = 0;
}

void UIOActivityInterface::setTarget(WeakReference<Activity, Activity> ref) {
	this->target.set(ref);
	this->targetSelectionTick = 0;
}

void UIOActivityInterface::splitTarget() {
	switch (this->type) {
		case UIO::USER_ACTION_TYPE::HOVERING:
			break;
		case UIO::USER_ACTION_TYPE::NOTHING:
			if (this->base.isValid() && this->target.isValid() && sameGroup(this->base, this->target)) {
				this->target.get()->disconnectFromParent();
			}
			break;
		default:
			break;
	}
}

void UIOActivityInterface::addLua(GameState& gameState, UIState& uiState) {
	static int32_t i = 0;
	glm::vec2 offset = glm::vec2(0.05f, -0.05f);
	if (this->type == UIO::USER_ACTION_TYPE::NOTHING && this->base.isValid()) {
		auto root = this->base.get()->getRoot();
		if (root.get()->getType() != ACTIVITY::TYPE::LUA) {
			//ACTIVITYSPAWNER::addLUA(gameState, this->base);
			root = root.get()->getRoot();
		}
		uiState.addUI(
			CONSTRUCTER::constructLuaInterface(root)
			.window("LUA", { static_cast<float>(i + 1) * offset + glm::vec2(-1.0f, 0.0f), static_cast<float>(i + 1) * offset + glm::vec2(-0.7f, 1.0f) },
					UIOWindow::TYPE::MINIMISE |
					UIOWindow::TYPE::RESIZEVERTICAL |
					UIOWindow::TYPE::RESIZEHORIZONTAL |
					UIOWindow::TYPE::RESIZE |
					UIOWindow::TYPE::MOVE |
					UIOWindow::TYPE::CLOSE)
			.get()
		);
	}

	i = (i + 1) % 10;
}

UIO::USER_ACTION_TYPE UIOActivityInterface::getType() {
	return this->type;
}

void UIOActivityInterface::updateCursorPos(glm::vec2 pos) {
	if (this->type == UIO::USER_ACTION_TYPE::HOVERING && this->cursor.isNotNull()) {
		if (this->cursor.get()->getType() == ACTIVITY::TYPE::ANCHOR) {
			WeakReference<Activity, Anchor> c = this->cursor;
			if (c.get()->hasChild()) {
				glm::ivec2 refVec2 = glm::ivec2(0, 0);
				for (auto child : c.get()->getChildren()) {
					refVec2 += child.get()->getOrigin();
				}
				refVec2 /= c.get()->getChildren().size();
				this->cursor.get()->forceMoveOriginUp(glm::ivec2(glm::floor(pos)) - refVec2);
			}
		}
		else {
			this->cursor.get()->forceMoveOriginUp(glm::ivec2(glm::floor(pos)) - this->cursor.get()->getOrigin());
		}
	}
}

void UIOActivityInterface::changeHoverActivityState(int32_t t) {
	if (this->cursor.isNotNull()) {
		this->cursor.get()->forceChangeActivityState(t);
	}
}

void UIOActivityInterface::pickUp(GameState& gameState, glm::vec2 pos) {
	switch (this->type) {
		case UIO::USER_ACTION_TYPE::NOTHING:
			{
				if (auto const& maybePick = gameState.staticWorld.getActivity(pos)) {
					auto const& pick = maybePick.value().get()->getRoot();
					if (this->base.isValid() && sameGroup(pick, this->base)) {
						return;
					}
					if (this->target.isValid() && sameGroup(pick, this->target)) {
						return;
					}

					if (this->cursor.isNotNull()) {
						this->cursor.deleteObject();
					}

					if (pick.get()->removeTracesUp(gameState)) {
						this->cursor.handle = pick.handle;
						this->type = UIO::USER_ACTION_TYPE::HOVERING;
					}
				}
				break;
			}
		case UIO::USER_ACTION_TYPE::HOVERING:
			break;
		default:
			break;
	}
}

void UIOActivityInterface::interact(GameState& gameState, glm::vec2 pos) {
	switch (this->type) {
		case UIO::USER_ACTION_TYPE::NOTHING:
			break;
		case UIO::USER_ACTION_TYPE::HOVERING:
			{
				if (this->cursor.isNotNull() && this->cursor.get()->fillTracesUp(gameState)) {
					WeakReference<Activity, Activity> linkTarget;
					if (this->target.isValid()) {
						linkTarget = this->target;
					}
					else if (this->base.isValid()) {
						linkTarget = this->base;
					}

					if (linkTarget.isNotNull()) {
						if (sameGroup(linkTarget, this->cursor)) {
							return;
						}
						Linker::link(gameState, linkTarget, this->cursor);
					}

					if (this->base.isValid()) {
						this->setBase(this->base);
					}

					this->cursor.clear();
					this->type = UIO::USER_ACTION_TYPE::NOTHING;
				}
				break;
			}
		default:
			break;
	}
}

void UIOActivityInterface::spawnHover(GameState& gameState, glm::ivec2 pos, ACTIVITY::TYPE activityType) {
	switch (this->type) {
		case UIO::USER_ACTION_TYPE::HOVERING:
			this->cursor.deleteObject();
			this->type = UIO::USER_ACTION_TYPE::NOTHING;
		case UIO::USER_ACTION_TYPE::NOTHING:
			this->type = UIO::USER_ACTION_TYPE::HOVERING;
			if (auto activity = ACTIVITYSPAWNER::spawn(gameState, pos, activityType)) {
				this->cursor = std::move(activity.value());
			}
			break;
		default:
			break;
	}

}

void UIOActivityInterface::rotateHover(ACTIVITY::ROT rot) {
	if (this->type == UIO::USER_ACTION_TYPE::HOVERING && this->cursor.isNotNull()) {
		auto t = this->cursor.get();
		auto center = t->getOrigin();
		t->rotateForcedUp(center, rot);
	}
}

int32_t UIOActivityInterface::addRenderInfo(GameState& gameState, RenderInfo& renderInfo, int32_t depth) {
	if (this->base.isValid()) {
		if (this->baseSelectionTick == 0) {
			this->baseSelectionTick = gameState.tick;
		}
		if (periodic(gameState.tick, 80, 40, -this->baseSelectionTick)) {
			std::vector<Activity*> members;
			this->base.get()->getTreeMembers(members);
			for (auto member : members) {
				if (member->getType() != ACTIVITY::TYPE::ANCHOR) {
					member->appendSelectionInfo(gameState, renderInfo, COLORS::GR::SELECTION);
				}
			}
		}
	}
	if (this->target.isValid()) {
		if (this->targetSelectionTick == 0) {
			this->targetSelectionTick = gameState.tick;
		}
		if (periodic(gameState.tick, 40, 20, -this->targetSelectionTick)) {
			this->target.get()->appendSelectionInfo(gameState, renderInfo, COLORS::GR::HIGHLIGHT);
		}
	}
	if (this->cursor.isNotNull()) {
		bool blocked = false;
		std::vector<Activity*> members;
		this->cursor.get()->getTreeMembers(members);
		for (auto member : members) {
			if (!member->canFillTracesLocal(gameState)) {
				blocked = true;
				break;
			}
		}

		glm::vec4 color;
		if (blocked) {
			color = COLORS::GR::BLOCKED;
		}
		else {
			color = COLORS::GR::HOVER;
		}

		this->cursor.get()->appendSelectionInfo(gameState, renderInfo, color);
	}
	return depth;
}

ScreenRectangle UIOActivityInterface::updateSize(ScreenRectangle newScreenRectangle) {
	newScreenRectangle.setHeight(0.0f);
	newScreenRectangle.setWidth(0.0f);

	this->screenRectangle = newScreenRectangle;

	return this->screenRectangle;
}
