#include "common.h"

#include "ActivityInteractor.h"
#include "GameState.h"
#include <iostream>

ActivityInteractor::ActivityInteractor() : ActivitySelector() {
	addBind({ CONTROLS::TEST_UP, CONTROLSTATE::CONTROLSTATE_DOWN }, [](GameState& gameState, LogicSequencer* self_) {
		auto self = static_cast<ActivityInteractor*>(self_);
		if (self->target.isValid()) {
			self->target.get()->applyCurrentMove(gameState, MOVEABLE::UP, 10);
		}
		return std::make_pair(CONTINUATION::CONTINUE, std::nullopt);
	});
	addBind({ CONTROLS::TEST_LEFT, CONTROLSTATE::CONTROLSTATE_DOWN }, [](GameState& gameState, LogicSequencer* self_) {
		auto self = static_cast<ActivityInteractor*>(self_);
		if (self->target.isValid()) {
			self->target.get()->applyCurrentMove(gameState, MOVEABLE::LEFT, 10);
		}
		return std::make_pair(CONTINUATION::CONTINUE, std::nullopt);
	});
	addBind({ CONTROLS::TEST_RIGHT, CONTROLSTATE::CONTROLSTATE_DOWN }, [](GameState& gameState, LogicSequencer* self_) {
		auto self = static_cast<ActivityInteractor*>(self_);
		if (self->target.isValid()) {
			self->target.get()->applyCurrentMove(gameState, MOVEABLE::RIGHT, 10);
		}
		return std::make_pair(CONTINUATION::CONTINUE, std::nullopt);
	});
	addBind({ CONTROLS::TEST_DOWN, CONTROLSTATE::CONTROLSTATE_DOWN }, [](GameState& gameState, LogicSequencer* self_) {
		auto self = static_cast<ActivityInteractor*>(self_);
		if (self->target.isValid()) {
			self->target.get()->applyCurrentMove(gameState, MOVEABLE::DOWN, 10);
		}
		return std::make_pair(CONTINUATION::CONTINUE, std::nullopt);
	});

}

