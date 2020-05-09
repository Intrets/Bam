#include "common.h"

#include "ActivityInteractor.h"
#include "GameState.h"
#include <iostream>

ActivityInteractor::ActivityInteractor() : ActivitySelector() {
	addBind({ CONTROLS::TEST_UP, CONTROLSTATE::CONTROLSTATE_DOWN }, [](GameState& gameState, LogicSequencer* self_) {
		auto self = static_cast<ActivityInteractor*>(self_);
		if (self->target.isValid()) {
			self->target.get()->applyMoveUp(gameState, MOVEABLE::UP, 10);
		}
		return std::make_pair(CONTINUATION::CONTINUE, std::nullopt);
	});
	addBind({ CONTROLS::TEST_LEFT, CONTROLSTATE::CONTROLSTATE_DOWN }, [](GameState& gameState, LogicSequencer* self_) {
		auto self = static_cast<ActivityInteractor*>(self_);
		if (self->target.isValid()) {
			self->target.get()->applyMoveUp(gameState, MOVEABLE::LEFT, 10);
		}
		return std::make_pair(CONTINUATION::CONTINUE, std::nullopt);
	});
	addBind({ CONTROLS::TEST_RIGHT, CONTROLSTATE::CONTROLSTATE_DOWN }, [](GameState& gameState, LogicSequencer* self_) {
		auto self = static_cast<ActivityInteractor*>(self_);
		if (self->target.isValid()) {
			self->target.get()->applyMoveUp(gameState, MOVEABLE::RIGHT, 10);
		}
		return std::make_pair(CONTINUATION::CONTINUE, std::nullopt);
	});
	addBind({ CONTROLS::TEST_DOWN, CONTROLSTATE::CONTROLSTATE_DOWN }, [](GameState& gameState, LogicSequencer* self_) {
		auto self = static_cast<ActivityInteractor*>(self_);
		if (self->target.isValid()) {
			self->target.get()->applyMoveUp(gameState, MOVEABLE::DOWN, 10);
		}
		return std::make_pair(CONTINUATION::CONTINUE, std::nullopt);
	});

	addBind({ CONTROLS::ACTION3, CONTROLSTATE::CONTROLSTATE_DOWN }, [](GameState& gameState, LogicSequencer* self_) {
		auto self = static_cast<ActivityInteractor*>(self_);
		if (self->target.isValid()) {
			self->target.get()->applyActivityLocal(gameState, 1, 10);
		}
		return std::make_pair(CONTINUATION::CONTINUE, std::nullopt);
	});

	addBind({ CONTROLS::ACTION4, CONTROLSTATE::CONTROLSTATE_DOWN }, [](GameState& gameState, LogicSequencer* self_) {
		auto self = static_cast<ActivityInteractor*>(self_);
		if (self->target.isValid()) {
			self->target.get()->applyActivityLocal(gameState, 2, 10);
		}
		return std::make_pair(CONTINUATION::CONTINUE, std::nullopt);
	});

	addBind({ CONTROLS::ROTATEL, CONTROLSTATE::CONTROLSTATE_PRESSED }, [](GameState& gameState, LogicSequencer* self_) {
		auto self = static_cast<ActivityInteractor*>(self_);
		if (self->target.isValid()) {
			auto center = self->target.get()->getOrigin();
			self->target.get()->rotateForcedUp(center, MOVEABLE::ROT::COUNTERCLOCKWISE);
		}
		return std::make_pair(CONTINUATION::CONTINUE, std::nullopt);
	});

	addBind({ CONTROLS::ROTATER, CONTROLSTATE::CONTROLSTATE_PRESSED }, [](GameState& gameState, LogicSequencer* self_) {
		auto self = static_cast<ActivityInteractor*>(self_);
		if (self->target.isValid()) {
			auto center = self->target.get()->getOrigin();
			self->target.get()->rotateForcedUp(center, MOVEABLE::ROT::CLOCKWISE);
		}
		return std::make_pair(CONTINUATION::CONTINUE, std::nullopt);
	});
}

