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

void BindHandler::appendRenderInfo(GameState& gameState, RenderInfo& renderInfo) {
	for (auto& logicSequence : logicSequences) {
		logicSequence->appendRenderInfo(gameState, renderInfo);
	}

	ScreenRectangle r;
	r.screenPixels = renderInfo.cameraInfo.viewPort;
	r.set({ 0.0f,0.0f }, { 0.5f, 0.5f });

	UI->updateSizeUp(r);
	UI->addRenderInfo(renderInfo);
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
	UI->clickTest(gameState.getPlayerCursorScreenSpace(), gameState.getPlayerCursorScreenSpaceD());
}

void BindHandler::addBind(CONTROLS control, CONTROLSTATE state, std::function<void(GameState&)> f) {
	binds[control][state].push_back(f);
}

BindHandler::BindHandler() {
	auto tileTest = std::make_unique<UIOWindowTileTest>();
	tileTest->nextRow();

	for (int i = 0; i < 4; i++) {
		std::unique_ptr<UIOWindowTest> n = std::make_unique<UIOWindowTest>();
		tileTest->add(std::move(n));
	}
	tileTest->nextRow();

	for (int i = 0; i < 8; i++) {
		std::unique_ptr<UIOWindowTest> n = std::make_unique<UIOWindowTest>();
		tileTest->add(std::move(n));
	}

	tileTest->nextRow();
	for (int i = 0; i < 3; i++) {
		std::unique_ptr<UIOWindowTest> n = std::make_unique<UIOWindowTest>();
		tileTest->add(std::move(n));
	}

	auto tileTest2 = std::make_unique<UIOWindowTileTest>();
	tileTest2->nextRow();

	for (int i = 0; i < 4; i++) {
		std::unique_ptr<UIOWindowTest> n = std::make_unique<UIOWindowTest>();
		tileTest2->add(std::move(n));
	}
	tileTest2->nextRow();

	for (int i = 0; i < 8; i++) {
		std::unique_ptr<UIOWindowTest> n = std::make_unique<UIOWindowTest>();
		tileTest2->add(std::move(n));
	}

	tileTest2->nextRow();
	for (int i = 0; i < 3; i++) {
		std::unique_ptr<UIOWindowTest> n = std::make_unique<UIOWindowTest>();
		tileTest2->add(std::move(n));
	}

	tileTest->add(std::move(tileTest2));
	//UI = std::move(tileTest);

	auto s = std::make_unique<UIOWindowResize>(std::move(tileTest));
	UI = std::move(s);

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
