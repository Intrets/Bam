#include "common.h"
#include "LogicSequencer.h"

#include "ControlState.h"
#include "GameState.h"
#include "RenderInfo.h"

void LogicSequencer::appendRenderInfo(GameState& gameState, RenderInfo& renderInfo) {
	if (next.has_value()) {
		next.value()->appendRenderInfo(gameState, renderInfo);
	}
	appendRenderInfoInternal(gameState, renderInfo);
}

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
	return CONTINUATION::STOP;
}

LogicSequencer::LogicSequencer() {
	blocking = true;
	auto exitBind = [](GameState& gameState, LogicSequencer* logicSequencer) {
		logicSequencer->exit(gameState);
		return std::make_pair(CONTINUATION::EXIT, std::nullopt);
	};
	addBind({ CONTROLS::TEST_EXIT, CONTROLSTATE::CONTROLSTATE_PRESSED }, exitBind);
}

LogicSequencer::~LogicSequencer() {
}
