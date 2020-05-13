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
#include "ActivityPlacer.h"
#include "ActivityLinker.h"
#include "ActivitySelector.h"
#include "ActivityInteractor.h"
#include "RenderInfo.h"
#include "ActivityEditor.h"

void BindHandler::appendRenderInfo(GameState& gameState, RenderInfo& renderInfo) {
	for (auto& logicSequence : logicSequences) {
		logicSequence->appendRenderInfo(gameState, renderInfo);
	}
}

void BindHandler::runBinds(ControlState& controlState, GameState& gameState) {
	for (auto& logicSequence : logicSequences) {
		logicSequence->runBinds(controlState, gameState);
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
	auto tools = std::make_unique<LogicSequencer>();

	auto placerTool = [](GameState& gameState, LogicSequencer* logicSequencer) {
		auto placer = std::make_unique<ActivityPlacer>();
		return std::make_pair(CONTINUATION::CONTINUE, std::make_optional(std::move(placer)));
	};

	auto linkerTool = [](GameState& gameState, LogicSequencer* logicSequencer) {
		auto placer = std::make_unique<ActivityLinker>();
		return std::make_pair(CONTINUATION::CONTINUE, std::make_optional(std::move(placer)));
	};

	auto activityInteractorTool = [](GameState& gameState, LogicSequencer* logicSequencer) {
		auto interactor = std::make_unique<ActivityInteractor>();
		return std::make_pair(CONTINUATION::CONTINUE, std::make_optional(std::move(interactor)));
	};

	auto activityEditorTool = [](GameState& gameState, LogicSequencer* logicSequencer) {
		auto interactor = std::make_unique<ActivityEditor>();
		return std::make_pair(CONTINUATION::CONTINUE, std::make_optional(std::move(interactor)));
	};

	tools->addBind({ CONTROLS::TOOL_1, CONTROLSTATE::CONTROLSTATE_PRESSED }, std::move(placerTool)
	);

	tools->addBind({ CONTROLS::TOOL_2, CONTROLSTATE::CONTROLSTATE_PRESSED }, std::move(linkerTool)
	);

	tools->addBind({ CONTROLS::TOOL_3, CONTROLSTATE::CONTROLSTATE_PRESSED }, std::move(activityInteractorTool)
	);

	tools->addBind({ CONTROLS::TOOL_4, CONTROLSTATE::CONTROLSTATE_PRESSED }, std::move(activityEditorTool)
	);

	logicSequences.push_back(std::move(tools));
}

BindHandler::~BindHandler() {
}
