#include "common.h"
#include "BindHandler.h"

#include <functional>
#include "ControlState.h"
#include "GameState.h"
#include "LogicSequencer.h"
#include <iostream>
#include "ActivityPlacer.h"
#include "ActivityLinker.h"
#include "ActivitySelector.h"
#include "ActivityInteractor.h"
#include "RenderInfo.h"
#include "ActivityLuaTest.h"

#include "UIOBasicWindow.h"
#include "UIOWindowTile.h"
#include "UIOScaling.h"
#include "UIOMoving.h"

void BindHandler::appendRenderInfo(GameState& gameState, RenderInfo& renderInfo) {
	for (auto& logicSequence : logicSequences) {
		logicSequence->appendRenderInfo(gameState, renderInfo);
	}

	UI.get()->addRenderInfo(renderInfo, 0);
}

void BindHandler::runBinds(ControlState& controlState, GameState& gameState) {
	for (auto& logicSequence : logicSequences) {
		logicSequence->runBinds(controlState, gameState);
	}

	for (int32_t i = 0; i < CONTROLS::CONTROLS_MAX; i++) {
		CONTROLS control = static_cast<CONTROLS>(i);
		CONTROLSTATE state = controlState.controlState[control];

		for (auto& bind : binds[control][state]) {
			bind(gameState);
		}
	}

	UI.get()->runBinds(controlState, gameState);
}

void BindHandler::addBind(CONTROLS control, CONTROLSTATE state, std::function<void(GameState&)> f) {
	binds[control][state].push_back(f);
}

BindHandler::BindHandler() {
	auto refMan = Locator<ReferenceManager<UIOBase>>::getService();
	auto tile = refMan->makeUniqueRef<UIOWindowTile>();
	auto tilePtr = tile.get();

	tilePtr->nextRow();

	for (int i = 0; i < 5; i++) {
		tilePtr->add(refMan->makeUniqueRef<UIOBasicWindow>());
	}

	tilePtr->nextRow();

	for (int i = 0; i < 2; i++) {
		tilePtr->add(refMan->makeUniqueRef<UIOBasicWindow>());
	}

	tilePtr->nextRow();

	for (int i = 0; i < 4; i++) {
		tilePtr->add(refMan->makeUniqueRef<UIOBasicWindow>());
	}

	tilePtr->nextRow();

	for (int i = 0; i < 3; i++) {
		tilePtr->add(refMan->makeUniqueRef<UIOBasicWindow>());
	}

	auto test = refMan->makeUniqueRef<UIOScaling>(std::move(tile));
	auto test2 = refMan->makeUniqueRef<UIOMoving>(std::move(test));

	UI = std::move(test2);

	ScreenRectangle r;

	r.set({ -0.25f,-0.25f }, { 0.25f, 0.25f });

	UI.get()->updateSize(r);




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

	auto activityLuaTestTool = [](GameState& gameState, LogicSequencer* logicSequencer) {
		auto interactor = std::make_unique<ActivityLuaTest>();
		return std::make_pair(CONTINUATION::CONTINUE, std::make_optional(std::move(interactor)));
	};


	tools->addBind({ CONTROLS::TOOL_1, CONTROLSTATE::CONTROLSTATE_PRESSED }, std::move(placerTool)
	);

	tools->addBind({ CONTROLS::TOOL_2, CONTROLSTATE::CONTROLSTATE_PRESSED }, std::move(linkerTool)
	);

	tools->addBind({ CONTROLS::TOOL_3, CONTROLSTATE::CONTROLSTATE_PRESSED }, std::move(activityInteractorTool)
	);

	tools->addBind({ CONTROLS::TOOL_4, CONTROLSTATE::CONTROLSTATE_PRESSED }, std::move(activityLuaTestTool)
	);

	logicSequences.push_back(std::move(tools));
}

BindHandler::~BindHandler() {
}
