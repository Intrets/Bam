#include "common.h"

#include "UIOActivitySelector.h"
#include "GameState.h"
#include "Platform.h"

void UIOActivitySelector::selectTarget(GameState& gameState) {
	if (this->type == SELECTION_TYPE::NOTHING) {
		auto maybeTarget = gameState.staticWorld.getActivity(glm::floor(gameState.getPlayerCursorWorldSpace()));
		if (maybeTarget.has_value()) {
			this->target.set(maybeTarget.value());
		}
	}
}

void UIOActivitySelector::expandTarget() {
	if (this->target.isValid()) {
		if (this->target.get()->parentRef.isNotNull()) {
			this->history.push_back(ManagedReference<Activity, Activity>(this->target.handle));
			this->target.set(this->target.get()->parentRef);
		}
	}
}

void UIOActivitySelector::spawnHover(GameState& gameState, glm::ivec2 pos, ACTIVITY::TYPE activityType) {
	switch (activityType) {
		case ACTIVITY::PLATFORM:
			this->target.set(Locator<ReferenceManager<Activity>>::getService()->makeRef<Platform>(gameState, glm::ivec2(6, 5), pos, false));
			break;
		default:
			break;
	}
}

void UIOActivitySelector::rotateHover(MOVEABLE::ROT rot) {
	if (this->type == SELECTION_TYPE::HOVERING && this->target.isValid()) {
		auto t = this->target.get();
		auto center = t->getOrigin();
		t->rotateForcedUp(center, rot);
	}
}

UIOActivitySelector::UIOActivitySelector(Handle self) {
	selfHandle = self;

	this->addGlobalBind({ CONTROLS::ACTION0, CONTROLSTATE::CONTROLSTATE_PRESSED }, [&](GameState& gameState, ControlState& controlState, UIOBase* self_) -> CallBackBindResult {
		switch (this->type) {
			case SELECTION_TYPE::NOTHING:
				{
					auto maybeTarget = gameState.staticWorld.getActivity(glm::floor(gameState.getPlayerCursorWorldSpace()));
					if (maybeTarget.has_value()) {
						this->type = SELECTION_TYPE::SELECTED;
						this->target.set(maybeTarget.value());
					}
					else {
						this->type = SELECTION_TYPE::HOVERING;
						this->spawnHover(gameState, glm::ivec2(gameState.getPlayerCursorWorldSpace()), ACTIVITY::TYPE::PLATFORM);
					}
					break;
				}
			case SELECTION_TYPE::HOVERING:
				{
					if (!this->target.isValid()) {
						this->target.unset();
						this->type = SELECTION_TYPE::NOTHING;
					}
					else if (this->target.get()->fillTracesUp(gameState)) {
						this->type = SELECTION_TYPE::SELECTED;
					}
					break;
				}
			case SELECTION_TYPE::SELECTED:
				{
					auto maybeTarget = gameState.staticWorld.getActivity(glm::floor(gameState.getPlayerCursorWorldSpace()));
					if (maybeTarget.has_value() && maybeTarget.value().handle == this->target.handle) {
						if (this->target.get()->removeTracesUp(gameState)) {
							this->target.set(maybeTarget.value());
							type = SELECTION_TYPE::HOVERING;
						}
					}
					else {
						this->target.unset();
						this->type = SELECTION_TYPE::NOTHING;
					}
					break;
				}
			default:
				break;
		}
		return BIND_RESULT::CONTINUE;
	});

	this->addGlobalBind({ CONTROLS::MOUSE_POS_CHANGED, CONTROLSTATE::CONTROLSTATE_PRESSED }, [&](GameState& gameState, ControlState& controlState, UIOBase* self_) -> CallBackBindResult {
		if (this->type == SELECTION_TYPE::HOVERING) {
			auto pos = glm::ivec2(glm::floor(gameState.getPlayerCursorWorldSpace()));
			this->target.get()->forceMoveOriginUp(pos - this->target.get()->getOrigin());
		}
		return BIND_RESULT::CONTINUE;
	});

	this->addGlobalBind({ CONTROLS::ROTATEL, CONTROLSTATE::CONTROLSTATE_PRESSED | CONTROLSTATE::CONTROLSTATE_REPEAT }, [&](GameState& gameState, ControlState& controlState, UIOBase* self_) -> CallBackBindResult {
		this->rotateHover(MOVEABLE::COUNTERCLOCKWISE);
		return BIND_RESULT::CONTINUE;
	});

	this->addGlobalBind({ CONTROLS::ROTATER, CONTROLSTATE::CONTROLSTATE_PRESSED | CONTROLSTATE::CONTROLSTATE_REPEAT }, [&](GameState& gameState, ControlState& controlState, UIOBase* self_) -> CallBackBindResult {
		this->rotateHover(MOVEABLE::CLOCKWISE);
		return BIND_RESULT::CONTINUE;
	});
}

ScreenRectangle UIOActivitySelector::updateSize(ScreenRectangle newScreenRectangle) {
	return newScreenRectangle;
}

int32_t UIOActivitySelector::addRenderInfo(GameState& gameState, RenderInfo& renderInfo, int32_t depth) {
	if (this->target.isValid()) {
		glm::vec4 color;
		if (this->target.get()->canFillTracesLocal(gameState)) {
			color = { 0.5, 1, 0.5, 0.5 };
		}
		else {
			color = { 1, 0.5, 0.5, 0.5 };
		}
		this->target.get()->appendSelectionInfo(gameState, renderInfo, color);
	}
	return depth;
}
