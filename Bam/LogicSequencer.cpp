#include "common.h"
#include "LogicSequencer.h"

#include "ControlState.h"
#include "GameState.h"

// returns if blocking
CONTINUATION LogicSequencer::runBinds(ControlState& controlState, GameState& gameState) {
	if (next.has_value()) {
		CONTINUATION nextResult = next.value()->runBinds(controlState, gameState);
		switch (nextResult) {
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

		auto bindResult = bind->second(gameState, this);
		switch (bindResult.first) {
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

		if (bindResult.second.has_value()) {
			next = std::move(bindResult.second);
			return CONTINUATION::STOP;
		}
	}
	return CONTINUATION::STOP;
}

LogicSequencer::LogicSequencer() {
	blocking = true;
	auto exitBind = [](GameState& gameState, LogicSequencer* logicSequencer) {
		return std::make_pair(CONTINUATION::EXIT, std::nullopt);
	};
	addBind({ CONTROLS::TEST_EXIT, CONTROLSTATE::CONTROLSTATE_PRESSED }, exitBind);
}

LogicSequencer::~LogicSequencer() {
}
