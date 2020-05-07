#include "common.h"
#include "LogicSequencer.h"

#include "ControlState.h"
#include "GameState.h"

// returns if blocking
CONTINUATION LogicSequencer::runBinds(ControlState& controlState, GameState& gameState) {
	if (next.has_value()) {
		CONTINUATION nextBlocking = next.value()->runBinds(controlState, gameState);
		switch (nextBlocking) {
			case CONTINUATION::EXIT:
				next.reset();
				break;
			case CONTINUATION::STOP:
				return CONTINUATION::STOP;
				break;
			case CONTINUATION::CONTINUE:
				break;
			default:
				break;
		}
	}
	for (BindControl bindControl : controlState.getBindControls()) {
		auto bind = binds.find(bindControl);
		if (bind == binds.end()) {
			continue;
		}

		auto maybeNext = bind->second(gameState);
		switch (maybeNext.first) {
			case CONTINUATION::EXIT:
				return CONTINUATION::EXIT;
				break;
			case CONTINUATION::STOP:
				return CONTINUATION::STOP;
				break;
			case CONTINUATION::CONTINUE:
				break;
			default:
				break;
		}

		if (maybeNext.second.has_value()) {
			next = std::move(maybeNext.second);
			return CONTINUATION::STOP;
		}
	}
	return CONTINUATION::STOP;
}

void LogicSequencer::addBind(BindControl bindControl, BindType bind) {
	binds.insert(std::make_pair(bindControl, bind));
}

LogicSequencer::LogicSequencer() {
	blocking = true;
	auto exitBind = [](GameState& gameState) {
		return std::make_pair(CONTINUATION::EXIT, std::nullopt);
	};
	addBind({ CONTROLS::TEST_EXIT, CONTROLSTATE::CONTROLSTATE_PRESSED }, exitBind);
}

LogicSequencer::~LogicSequencer() {
}
