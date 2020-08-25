#include "common.h"

#include "UIOActivityInterface.h"

#include "Colors.h"
#include "GameState.h"
#include "Platform.h"
#include "Piston.h"
#include "UIOCallBackParams.h"

UIOActivityInterface::UIOActivityInterface(Handle self) {
	this->selfHandle = self;


	this->addFocussedBind({ ControlState::CONTROLS::ACTION0, ControlState::CONTROLSTATE_PRESSED }, [](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
	{
		auto self = static_cast<UIOActivityInterface*>(self_);
		self->interact(params.gameState, params.uiState.getCursorPositionWorld());
		return BIND_RESULT::CONTINUE;
	});

	this->addFocussedBind({ ControlState::CONTROLS::MOUSE_POS_CHANGED }, [](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
	{
		auto self = static_cast<UIOActivityInterface*>(self_);
		self->updateCursorPos(params.uiState.getCursorPositionWorld());
		return BIND_RESULT::CONTINUE;
	});

	this->addFocussedBind({ ControlState::CONTROLS::ACTION1 }, [](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
	{
		auto self = static_cast<UIOActivityInterface*>(self_);
		self->spawnHover(params.gameState, params.uiState.getCursorPositionWorld(), Activity::TYPE::PISTON);
		return BIND_RESULT::CONTINUE;
	});
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

void UIOActivityInterface::updateCursorPos(glm::vec2 pos) {
	if (this->type == USER_ACTION_TYPE::HOVERING && this->cursor.isValid()) {
		this->cursor.get()->forceMoveOriginUp(glm::ivec2(pos) - this->cursor.get()->getOrigin());
	}
}

void UIOActivityInterface::interact(GameState& gameState, glm::vec2 pos) {
	switch (this->type) {
		case USER_ACTION_TYPE::NOTHING:
			break;
		case USER_ACTION_TYPE::HOVERING:
			{
				if (!this->cursor.isValid()) {
					this->cursor.unset();
					this->type = USER_ACTION_TYPE::NOTHING;
				}
				else if (this->cursor.get()->fillTracesUp(gameState)) {
					// TODO link with target
					this->cursor.unset();
					this->type = USER_ACTION_TYPE::NOTHING;
				}
				break;
			}
		case USER_ACTION_TYPE::SELECTED:
			this->cursor.unset();
			this->type = USER_ACTION_TYPE::NOTHING;
			break;
		default:
			break;
	}
}

void UIOActivityInterface::spawnHover(GameState& gameState, glm::ivec2 pos, Activity::TYPE activityType) {
	if (this->type != USER_ACTION_TYPE::NOTHING) {
		return;
	}

	this->type = USER_ACTION_TYPE::HOVERING;

	switch (activityType) {
		case Activity::TYPE::PLATFORM:
			this->cursor.set(Locator<ReferenceManager<Activity>>::get()->makeRef<Platform>(gameState, glm::ivec2(6, 5), pos, false));
			break;
		case Activity::TYPE::PISTON:
			this->cursor.set(Locator<ReferenceManager<Activity>>::get()->makeRef<Piston>(gameState, pos, Activity::DIR::DOWN, false));
			break;
		default:
			break;
	}
}

void UIOActivityInterface::rotateHover(Activity::ROT rot) {
	if (this->type == USER_ACTION_TYPE::HOVERING && this->cursor.isValid()) {
		auto t = this->cursor.get();
		auto center = t->getOrigin();
		t->rotateForcedUp(center, rot);
	}
}

int32_t UIOActivityInterface::addRenderInfo(GameState const& gameState, RenderInfo& renderInfo, int32_t depth) {
	if (this->base.isValid()) {
		if (this->baseSelectionTick == 0) {
			this->baseSelectionTick = gameState.tick;
		}
		if (periodic(gameState.tick, 80, 40, -this->baseSelectionTick)) {
			this->base.get()->appendSelectionInfo(gameState, renderInfo, COLORS::GR::SELECTION);
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
	return depth;
}

ScreenRectangle UIOActivityInterface::updateSize(ScreenRectangle newScreenRectangle) {
	newScreenRectangle.setHeight(0.0f);
	newScreenRectangle.setWidth(0.0f);

	this->screenRectangle = newScreenRectangle;

	return this->screenRectangle;
}
