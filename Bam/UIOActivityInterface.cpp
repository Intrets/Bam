#include "common.h"

#include "UIOActivityInterface.h"

#include "Colors.h"
#include "GameState.h"
#include "Platform.h"
#include "Piston.h"
#include "UIOCallBackParams.h"
#include "Linker.h"
#include "ActivityHelpers.h"

UIOActivityInterface::UIOActivityInterface(Handle self) {
	this->selfHandle = self;

	this->addFocussedBind({ ControlState::CONTROLS::CANCEL }, [](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
	{
		auto self = static_cast<UIOActivityInterface*>(self_);
		self->exit();
		return BIND_RESULT::CONTINUE | BIND_RESULT::HIDE;
	});

	this->addFocussedBind({ ControlState::CONTROLS::ROTATER, ControlState::CONTROLSTATE_PRESSED, ControlState::NONE }, [](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
	{
		auto self = static_cast<UIOActivityInterface*>(self_);
		self->rotateHover(Activity::ROT::CLOCKWISE);
		return BIND_RESULT::CONTINUE;
	});

	this->addFocussedBind({ ControlState::CONTROLS::ROTATER, ControlState::CONTROLSTATE_PRESSED, ControlState::SHIFT }, [](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
	{
		auto self = static_cast<UIOActivityInterface*>(self_);
		self->rotateHover(Activity::ROT::COUNTERCLOCKWISE);
		return BIND_RESULT::CONTINUE;
	});

	this->addGameWorldBind({ ControlState::CONTROLS::ACTION0 }, [](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
	{
		auto self = static_cast<UIOActivityInterface*>(self_);
		self->interact(params.gameState, params.uiState.getCursorPositionWorld());
		return BIND_RESULT::CONTINUE;
	});

	this->addGameWorldBind({ ControlState::CONTROLS::ACTION1 }, [](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
	{
		auto self = static_cast<UIOActivityInterface*>(self_);
		self->cancel();
		return BIND_RESULT::CONTINUE;
	});


	this->addGlobalBind({ ControlState::CONTROLS::MOUSE_POS_CHANGED }, [](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
	{
		auto self = static_cast<UIOActivityInterface*>(self_);
		self->updateCursorPos(params.uiState.getCursorPositionWorld());
		return BIND_RESULT::CONTINUE;
	});
}

void UIOActivityInterface::exit() {
	this->cursor.deleteObject();
	this->target.unset();
	this->base.unset();
	this->type = USER_ACTION_TYPE::NOTHING;
}

void UIOActivityInterface::cancel() {
	switch (this->type) {
		case USER_ACTION_TYPE::HOVERING:
			if (this->cursor.isNotNull()) {
				this->cursor.deleteObject();
			}
			this->type = USER_ACTION_TYPE::NOTHING;
			break;
		case USER_ACTION_TYPE::NOTHING:
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
		case USER_ACTION_TYPE::HOVERING:
			break;
		case USER_ACTION_TYPE::NOTHING:
			if (this->base.isValid() && this->target.isValid() && sameGroup(this->base, this->target)) {
				this->target.get()->disconnectFromParent();
			}
			break;
		default:
			break;
	}
}

void UIOActivityInterface::updateCursorPos(glm::vec2 pos) {
	if (this->type == USER_ACTION_TYPE::HOVERING && this->cursor.isNotNull()) {
		this->cursor.get()->forceMoveOriginUp(glm::ivec2(glm::floor(pos)) - this->cursor.get()->getOrigin());
	}
}

void UIOActivityInterface::interact(GameState& gameState, glm::vec2 pos) {
	switch (this->type) {
		case USER_ACTION_TYPE::NOTHING:
			break;
		case USER_ACTION_TYPE::HOVERING:
			{
				this->type = USER_ACTION_TYPE::NOTHING;
				if (this->cursor.isNotNull() && this->cursor.get()->fillTracesUp(gameState)) {
					WeakReference<Activity, Activity> linkTarget;
					if (this->target.isValid()) {
						linkTarget = this->target;
					}
					else if (this->base.isValid()) {
						linkTarget = this->base;
					}

					if (linkTarget.isNotNull()) {
						Linker::link(gameState, linkTarget, this->cursor);
					}

					if (this->base.isValid()) {
						this->setBase(this->base);
					}

					this->cursor.clear();
				}
				break;
			}
		default:
			break;
	}
}

void UIOActivityInterface::spawnHover(GameState& gameState, glm::ivec2 pos, Activity::TYPE activityType) {
	switch (this->type) {
		case USER_ACTION_TYPE::HOVERING:
			this->cursor.deleteObject();
			this->type = USER_ACTION_TYPE::NOTHING;
		case USER_ACTION_TYPE::NOTHING:
			this->type = USER_ACTION_TYPE::HOVERING;
			switch (activityType) {
				case Activity::TYPE::PLATFORM:
					this->cursor = Locator<ReferenceManager<Activity>>::get()->makeUniqueRef<Platform>(gameState, glm::ivec2(6, 5), pos, false);
					break;
				case Activity::TYPE::PISTON:
					this->cursor = Locator<ReferenceManager<Activity>>::get()->makeUniqueRef<Piston>(gameState, pos, Activity::DIR::DOWN, false);
					break;
				default:
					break;
			}
			break;
		default:
			break;
	}

}

void UIOActivityInterface::rotateHover(Activity::ROT rot) {
	if (this->type == USER_ACTION_TYPE::HOVERING && this->cursor.isNotNull()) {
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
				if (member->getType() != Activity::TYPE::ANCHOR) {
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
