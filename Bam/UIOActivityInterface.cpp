#include "common.h"

#include "UIOActivityInterface.h"

#include "Colors.h"
#include "GameState.h"
#include "Platform.h"
#include "Piston.h"
#include "PlayerState.h"
#include "Linker.h"
#include "ActivityHelpers.h"
#include "PlayerState.h"
#include "Anchor.h"
#include "ActivitySpawner.h"
#include "UIOConstructLuaInterface.h"
#include "UIOList.h"
#include "ActivityCopier.h"

UIOActivityInterface::UIOActivityInterface(Handle self) {
	this->selfHandle = self;

	this->addFocussedBind({ CONTROL::KEY::ACTION_PICK }, [](PlayerState& playerState, UIOBase* self_) -> CallBackBindResult
	{
		auto self = static_cast<UIOActivityInterface*>(self_);
		self->copy(playerState.gameState, playerState.uiState.getCursorPositionWorld());
		return BIND::CONTINUE;
	});

	this->addFocussedBind({ CONTROL::KEY::CANCEL }, [](PlayerState& playerState, UIOBase* self_) -> CallBackBindResult
	{
		auto self = static_cast<UIOActivityInterface*>(self_);
		self->exit();
		return BIND::RESULT::CONTINUE | BIND::RESULT::HIDE;
	});

	this->addFocussedBind({ CONTROL::KEY::ROTATER, CONTROL::STATE::PRESSED, CONTROL::MODIFIER::NONE }, [](PlayerState& playerState, UIOBase* self_) -> CallBackBindResult
	{
		auto self = static_cast<UIOActivityInterface*>(self_);
		self->rotateHover(ACTIVITY::ROT::CLOCKWISE);
		return BIND::RESULT::CONTINUE | BIND::RESULT::CONSUME;
	});

	this->addFocussedBind({ CONTROL::KEY::ROTATER, CONTROL::STATE::PRESSED, CONTROL::MODIFIER::SHIFT }, [](PlayerState& playerState, UIOBase* self_) -> CallBackBindResult
	{
		auto self = static_cast<UIOActivityInterface*>(self_);
		self->rotateHover(ACTIVITY::ROT::COUNTERCLOCKWISE);
		return BIND::RESULT::CONTINUE | BIND::RESULT::CONSUME;
	});

	this->addGameWorldBind({ CONTROL::KEY::ACTION1 }, [](PlayerState& playerState, UIOBase* self_) -> CallBackBindResult
	{
		auto self = static_cast<UIOActivityInterface*>(self_);
		self->cancel(false);
		return BIND::RESULT::CONTINUE | BIND::RESULT::CONSUME;
	});

	this->addGameWorldBind({ CONTROL::KEY::ACTION_USE }, [](PlayerState& playerState, UIOBase* self_) -> CallBackBindResult
	{
		auto self = static_cast<UIOActivityInterface*>(self_);
		self->pickUp(playerState.gameState, playerState.uiState.getCursorPositionWorld());
		return BIND::RESULT::CONTINUE | BIND::RESULT::CONSUME | BIND::RESULT::FOCUS;
	});

	this->addGameWorldBind({ CONTROL::KEY::ACTION_ACTIVATE, CONTROL::STATE::PRESSED | CONTROL::STATE::REPEAT, CONTROL::MODIFIER::NONE }, [](PlayerState& playerState, UIOBase* self_) -> CallBackBindResult
	{
		auto self = static_cast<UIOActivityInterface*>(self_);
		self->changeHoverActivityState(2);
		return BIND::RESULT::CONTINUE | BIND::RESULT::CONSUME;
	});

	this->addGameWorldBind({ CONTROL::KEY::ACTION_ACTIVATE, CONTROL::STATE::PRESSED | CONTROL::STATE::REPEAT, CONTROL::MODIFIER::SHIFT }, [](PlayerState& playerState, UIOBase* self_) -> CallBackBindResult
	{
		auto self = static_cast<UIOActivityInterface*>(self_);
		self->changeHoverActivityState(1);
		return BIND::RESULT::CONTINUE | BIND::RESULT::CONSUME;
	});

	this->addGlobalBind({ CONTROL::KEY::MOUSE_POS_CHANGED }, [](PlayerState& playerState, UIOBase* self_) -> CallBackBindResult
	{
		auto self = static_cast<UIOActivityInterface*>(self_);
		self->updateCursorPos(playerState.uiState.getCursorPositionWorld());
		return BIND::RESULT::CONTINUE;
	});

	this->addFocussedBind({ CONTROL::KEY::ACTION_DELETE }, [](PlayerState& playerState, UIOBase* self_) -> CallBackBindResult
	{
		auto self = static_cast<UIOActivityInterface*>(self_);
		self->splitTarget();
		return BIND::RESULT::CONTINUE | BIND::RESULT::CONSUME;
	});
}

void UIOActivityInterface::exit() {
	this->cursor.deleteObject();
	this->target.unset();
	this->type = UIO::USER_ACTION_TYPE::NOTHING;
}

void UIOActivityInterface::cancel(bool full) {
	if (full) {
		this->type = UIO::USER_ACTION_TYPE::NOTHING;
		this->cursor.deleteObject();
		this->target.unset();
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
				break;
			default:
				break;
		}
	}
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
			if (auto targetRef = this->target.getRef()) {
				auto targetRoot = targetRef.get()->getRootRef();

				targetRef.get()->disconnectFromParent();

				for (auto& member : targetRoot.get()->getTreeMembers()) {
					member->memberCache.invalidateMembers();
				}

				for (auto& member : targetRef.get()->getTreeMembers()) {
					member->memberCache.invalidateRoot();
				}
			}
			break;
		default:
			break;
	}
}

ManagedReference<Activity, Activity> const& UIOActivityInterface::getTarget() {
	return this->target;
}

UIO::USER_ACTION_TYPE UIOActivityInterface::getType() {
	return this->type;
}

void UIOActivityInterface::updateCursorPos(glm::vec2 pos) {
	if (this->type == UIO::USER_ACTION_TYPE::HOVERING && this->cursor.isNotNull()) {
		if (this->cursor.get()->getType() == ACTIVITY::TYPE::ANCHOR) {
			WeakReference<Activity, Anchor> c = this->cursor.as<Anchor>();
			if (c.get()->hasChild()) {
				glm::ivec2 refVec2 = glm::ivec2(0, 0);
				for (auto& child : c.get()->getChildren()) {
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

void UIOActivityInterface::copy(GameState& gameState, glm::vec2 pos) {
	switch (this->type) {
		case UIO::USER_ACTION_TYPE::NOTHING:
			{
				if (auto const& maybePick = gameState.staticWorld.getActivity(pos)) {
					this->cursor = ACTIVITYCOPIER::copy(gameState.getActivityManager(), maybePick.value());
					this->type = UIO::USER_ACTION_TYPE::HOVERING;
				}
				break;
			}
		case UIO::USER_ACTION_TYPE::HOVERING:
			break;
		default:
			break;
	}
}

void UIOActivityInterface::pickUp(GameState& gameState, glm::vec2 pos) {
	switch (this->type) {
		case UIO::USER_ACTION_TYPE::NOTHING:
			{
				if (auto const& maybePick = gameState.staticWorld.getActivity(pos)) {
					auto const& pick = maybePick.value().get()->getRootRef();
					if (auto targetRef = this->target.getRef()) {
						if (sameGroup(pick, targetRef)) {
							return;
						}
					}

					if (this->cursor.isNotNull()) {
						this->cursor.deleteObject();
					}

					if (pick.get()->removeTracesUp(gameState)) {
						this->cursor = pick;
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
					if (auto linkTarget = this->target.getRef()) {
						if (sameGroup(linkTarget, this->cursor)) {
							return;
						}
						if (Linker::link(gameState, linkTarget, this->cursor)) {
							this->cursor.clear();
						}
						else {
							assert(0);
						}
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
			[[fallthrough]];
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
	if (auto targetRef = this->target.getRef()) {
		if (this->baseSelectionTick == 0) {
			this->baseSelectionTick = gameState.tick;
		}
		if (periodic(gameState.tick, 80, 40, -this->baseSelectionTick)) {
			for (auto member : targetRef.get()->getRootPtr()->getTreeMembers()) {
				if (member->getType() != ACTIVITY::TYPE::ANCHOR) {
					member->appendSelectionInfo(gameState, renderInfo, COLORS::GR::SELECTION);
				}
			}
		}

		if (this->targetSelectionTick == 0) {
			this->targetSelectionTick = gameState.tick;
		}
		if (periodic(gameState.tick, 40, 20, -this->targetSelectionTick)) {
			targetRef.get()->appendSelectionInfo(gameState, renderInfo, COLORS::GR::HIGHLIGHT);
		}
	}
	if (this->cursor.isNotNull()) {
		bool blocked = false;
		for (auto member : this->cursor.get()->getTreeMembers()) {
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
