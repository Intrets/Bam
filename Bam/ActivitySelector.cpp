#include "common.h"

#include "ActivitySelector.h"
#include "GameState.h"
#include "LogicSequencer.h"

ActivitySelector::ActivitySelector() {
	addBind({ CONTROLS::ACTION0, CONTROLSTATE::CONTROLSTATE_PRESSED }, [](GameState& gameState, LogicSequencer* self_) {
		auto self = static_cast<ActivitySelector*>(self_);
		self->selectTarget(gameState);
		return std::make_pair(CONTINUATION::CONTINUE, std::nullopt);
	});
	// expand selection
	addBind({ CONTROLS::ACTION5, CONTROLSTATE::CONTROLSTATE_PRESSED }, [](GameState& gameState, LogicSequencer* self_) {
		auto self = static_cast<ActivitySelector*>(self_);
		self->expandTarget();
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

void ActivitySelector::selectTarget(GameState& gameState) {
	auto maybeTarget = gameState.staticWorld.getActivity(gameState.getPlayerCursorWorldSpace());
	if (maybeTarget.has_value()) {
		target.set(maybeTarget.value());
	}
}

void ActivitySelector::expandTarget() {
	if (target.isValid()) {
		if (target.get()->parentRef.isNotNull()) {
			history.push_back(std::make_unique<ManagedReference<Activity, Activity>>(target.handle));
			WeakReference<Activity, Activity> newTarget = target.get()->parentRef;
			target.set(newTarget);
		}
	}
}

void ActivitySelector::appendRenderInfoInternal(GameState& gameState, RenderInfo& renderInfo) {
	if (target.isValid()) {
		target.get()->appendSelectionInfo(gameState, renderInfo);
	}
}
