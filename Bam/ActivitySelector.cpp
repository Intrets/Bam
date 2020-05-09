#include "common.h"

#include "ActivitySelector.h"
#include "GameState.h"
#include "LogicSequencer.h"

ActivitySelector::ActivitySelector() {
	addBind({ CONTROLS::ACTION0, CONTROLSTATE::CONTROLSTATE_PRESSED }, [](GameState& gameState, LogicSequencer* self_) {
		auto self = static_cast<ActivitySelector*>(self_);
		auto maybeTarget = gameState.staticWorld.getActivity(gameState.getPlayerCursorWorldSpace());
		if (maybeTarget.has_value()) {
			self->target.set(maybeTarget.value());
		}
		return std::make_pair(CONTINUATION::CONTINUE, std::nullopt);
	});
	// expand selection
	addBind({ CONTROLS::ACTION5, CONTROLSTATE::CONTROLSTATE_PRESSED }, [](GameState& gameState, LogicSequencer* self_) {
		auto self = static_cast<ActivitySelector*>(self_);
		if (self->target.isValid()) {
			if (self->target.get()->parentRef.isNotNull()) {
				self->history.push_back(std::make_unique<ManagedReference<Activity, Activity>>(self->target.handle));
				WeakReference<Activity, Activity> newTarget = self->target.get()->parentRef;
				self->target.set(newTarget);
			}
		}
		return std::make_pair(CONTINUATION::CONTINUE, std::nullopt);
	});
	// shrink selection
	addBind({ CONTROLS::ACTION6, CONTROLSTATE::CONTROLSTATE_PRESSED }, [](GameState& gameState, LogicSequencer* self_) {
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
