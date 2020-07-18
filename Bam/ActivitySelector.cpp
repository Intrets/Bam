#include "common.h"

#include "ActivitySelector.h"
#include "GameState.h"
#include "RenderInfo.h"
#include <sstream>
#include "StringHelpers.h"
#include "Platform.h"

ActivitySelector::ActivitySelector() {
}

int32_t ActivitySelector::addRenderInfo(GameState& gameState, RenderInfo& renderInfo, int32_t depth) {
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

void ActivitySelector::selectTarget(GameState& gameState, glm::vec2 pos) {
	switch (this->type) {
		case SELECTION_TYPE::NOTHING:
			{
				auto maybeTarget = gameState.staticWorld.getActivity(glm::floor(pos));
				if (maybeTarget.has_value()) {
					this->type = SELECTION_TYPE::SELECTED;
					this->target.set(maybeTarget.value());
				}
				else {
					this->type = SELECTION_TYPE::HOVERING;
					this->spawnHover(gameState, glm::ivec2(pos), ACTIVITY::TYPE::PLATFORM);
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
				auto maybeTarget = gameState.staticWorld.getActivity(glm::floor(pos));
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
	if (this->type == SELECTION_TYPE::NOTHING) {
		auto maybeTarget = gameState.staticWorld.getActivity(glm::floor(pos));
		if (maybeTarget.has_value()) {
			this->target.set(maybeTarget.value());
		}
	}
}

void ActivitySelector::expandTarget() {
	if (this->target.isValid()) {
		if (this->target.get()->parentRef.isNotNull()) {
			this->history.push_back(ManagedReference<Activity, Activity>(this->target.handle));
			this->target.set(this->target.get()->parentRef);
		}
	}
}

void ActivitySelector::shrinkTarget() {
	while (!this->history.empty() && !this->history.back().isValid()) {
		this->history.pop_back();
	}
	if (!this->history.empty()) {
		this->target.set(this->history.back().handle);
		this->history.pop_back();
	}
}

void ActivitySelector::spawnHover(GameState& gameState, glm::ivec2 pos, ACTIVITY::TYPE activityType) {
	switch (activityType) {
		case ACTIVITY::PLATFORM:
			this->target.set(Locator<ReferenceManager<Activity>>::get()->makeRef<Platform>(gameState, glm::ivec2(6, 5), pos, false));
			break;
		default:
			break;
	}
}

void ActivitySelector::rotateHover(MOVEABLE::ROT rot) {
	if (this->type == SELECTION_TYPE::HOVERING && this->target.isValid()) {
		auto t = this->target.get();
		auto center = t->getOrigin();
		t->rotateForcedUp(center, rot);
	}
}
