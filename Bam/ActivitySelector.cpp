#include "common.h"

#include "ActivitySelector.h"
#include "GameState.h"
#include "LogicSequencer.h"

ActivitySelector::ActivitySelector() {
	addBind({ CONTROLS::ACTION0, CONTROLSTATE::CONTROLSTATE_PRESSED }, [](GameState& gameState, LogicSequencer* self_) {
		auto self = static_cast<ActivitySelector*>(self_);
		auto maybeTarget = gameState.staticWorld.getBlock(gameState.getPlayerCursorWorldSpace());
		if (maybeTarget.first == 1) {
			self->target.set(maybeTarget.second);
		}
		return std::make_pair(CONTINUATION::CONTINUE, std::nullopt);
	});
	// expand selection
	addBind({ CONTROLS::ACTION3, CONTROLSTATE::CONTROLSTATE_PRESSED }, [](GameState& gameState, LogicSequencer* self_) {
		auto self = static_cast<ActivitySelector*>(self_);
		if (self->target.isValid()) {
			if (self->target.get()->parentRef) {
				self->history.push_back(std::make_unique<ManagedReference<Activity, Activity>>(self->target.handle));
				WeakReference<Activity, Activity> newTarget = self->target.get()->parentRef;
				self->target.set(newTarget);
			}
		}
		return std::make_pair(CONTINUATION::CONTINUE, std::nullopt);
	});
	// shrink selection
	addBind({ CONTROLS::ACTION4, CONTROLSTATE::CONTROLSTATE_PRESSED }, [](GameState& gameState, LogicSequencer* self_) {
		auto self = static_cast<ActivitySelector*>(self_);
		while (!self->history.empty() && !self->history.back()->isValid()) {
			self->history.pop_back();
		}
		if (!self->history.empty()) {
			self->target.set(self->history.back()->handle);
			self->history.pop_back();
		}
		return std::make_pair(CONTINUATION::CONTINUE, std::nullopt);
	});
}

void ActivitySelector::appendRenderInfoInternal(GameState& gameState, RenderInfo& renderInfo) {
	if (target.isValid()) {
		target.get()->appendSelectionInfo(gameState, renderInfo);
	}
}
