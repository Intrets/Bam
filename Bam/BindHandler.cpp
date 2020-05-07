#include "common.h"
#include "BindHandler.h"

#include <functional>
//#include "ReferenceManager.h"
//#include "Activity.h"
//#include "Platform.h"
#include "ControlState.h"
#include "GameState.h"
#include "LogicSequencer.h"
#include <iostream>

void BindHandler::runBinds(ControlState& controlState, GameState& gameState) {
	for (auto& logicSequence : logicSequences) {
		auto blocking = logicSequence->runBinds(controlState, gameState);
		if (blocking != CONTINUATION::CONTINUE) {
			return;
		}
	}

	for (int i = 0; i < CONTROLS::CONTROLS_MAX; i++) {
		CONTROLS control = static_cast<CONTROLS>(i);
		CONTROLSTATE state = controlState.controlState[control];

		for (auto& bind : binds[control][state]) {
			bind(gameState);
		}
	}
}

void BindHandler::addBind(CONTROLS control, CONTROLSTATE state, std::function<void(GameState&)> f) {
	binds[control][state].push_back(f);
}

BindHandler::BindHandler() {
	auto test = std::make_unique<LogicSequencer>(false);
	test->addBind({ CONTROLS::TEST_UP, CONTROLSTATE::CONTROLSTATE_PRESSED }, [](GameState& gameState) -> LogicSequencer::MaybeSequencer {
		std::cout << "TEST_UP\n";
		auto test = std::make_unique<LogicSequencer>(false);
		test->addBind({ CONTROLS::TEST_RIGHT, CONTROLSTATE::CONTROLSTATE_PRESSED }, [](GameState& gameState) -> LogicSequencer::MaybeSequencer {
			std::cout << "TEST_RIGHT\n";
			return std::make_pair(CONTINUATION::STOP, std::nullopt);
		});
		return std::make_pair(CONTINUATION::CONTINUE, std::make_optional(std::move(test)));
	});
	logicSequences.push_back(std::move(test));
}

BindHandler::~BindHandler() {
}
