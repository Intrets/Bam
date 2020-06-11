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
#include "UIOTextEdit.h"
#include "UIOTextDisplay.h"

void BindHandler::appendRenderInfo(GameState& gameState, RenderInfo& renderInfo) {
	for (auto& logicSequence : logicSequences) {
		logicSequence->appendRenderInfo(gameState, renderInfo);
	}

	//auto r = UI.get()->screenRectangle; 
	//r.screenPixels = renderInfo.frameSize;

	int32_t depth = 0;
	for (auto& UI : UIs) {
		depth = UI.get()->addRenderInfo(renderInfo, depth);
	}
}

void BindHandler::runBinds(ControlState& controlState, GameState& gameState) {
	for (auto& logicSequence : logicSequences) {
		logicSequence->runBinds(controlState, gameState);
	}

	for (auto& UI : UIs) {
		UI.get()->runBinds(controlState, gameState);
	}
}

BindHandler::BindHandler(GLFWwindow* window) {
	auto refMan = Locator<ReferenceManager<UIOBase>>::getService();
	auto text = refMan->makeUniqueRef<UIOTextEdit>();

	auto test = refMan->makeUniqueRef<UIOScaling>(std::move(text));
	auto test2 = refMan->makeUniqueRef<UIOMoving>(std::move(test));

	ScreenRectangle r;
	r.set({ -0.25f,-0.25f }, { 0.25f, 0.25f });
	int32_t x;
	int32_t y;
	glfwGetWindowSize(window, &x, &y);
	r.screenPixels = glm::ivec2(x, y);

	test2.get()->updateSize(r);

	UIs.push_back(std::move(test2));

	auto textDisplay = refMan->makeUniqueRef<UIOTextDisplay>();
	textDisplay.get()->setText("tesg 123\n\n\n12323");

	auto t2 = refMan->makeUniqueRef<UIOMoving>(std::move(textDisplay));

	r.set({ -1.0f, 0.0f }, { 0.0f, -1.0f });

	t2.get()->updateSize(r);

	UIs.push_back(std::move(t2));


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
