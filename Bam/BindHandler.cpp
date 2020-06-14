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
#include "UIOTextEdit.h"
#include "UIOTextDisplay.h"
#include "UIOWindow.h"
#include "UIOConstrainSize.h"
#include "UIOFreeSize.h"
#include "UIOPad.h"
#include "UIOHotbar.h"

void BindHandler::updateWindowSize(GLFWwindow* window) {
	int32_t x;
	int32_t y;
	glfwGetWindowSize(window, &x, &y);

	ScreenRectangle r;
	r.screenPixels = glm::ivec2(x, y);
	r.set({ -1,-1 }, { 1,1 });
	for (auto& UI : UIs) {
		UI.get()->updateSize(r);
	}
}

void BindHandler::appendRenderInfo(GameState& gameState, RenderInfo& renderInfo) {
	for (auto& logicSequence : logicSequences) {
		logicSequence->appendRenderInfo(gameState, renderInfo);
	}

	int32_t depth = 0;
	for (auto& UI : UIs) {
		depth = UI.get()->addRenderInfo(renderInfo, depth);
	}
}

void BindHandler::runBinds(ControlState& controlState, GameState& gameState) {
	for (auto& logicSequence : logicSequences) {
		logicSequence->runBinds(controlState, gameState);
	}

	for (auto it = UIs.begin(); it != UIs.end(); it++) {
		auto& UI = *it;
		if (UI.get()->runBinds(controlState, gameState)) {
			auto temp = std::move(UI);
			UIs.erase(it);
			UIs.push_front(std::move(temp));
			break;
		}
	}
}

BindHandler::BindHandler(GLFWwindow* window) {
	ScreenRectangle r;
	r.set({ -1,-1 }, { 1,1 });
	int32_t x;
	int32_t y;
	glfwGetWindowSize(window, &x, &y);
	r.screenPixels = glm::ivec2(x, y);

	auto refMan = Locator<ReferenceManager<UIOBase>>::getService();

	{
		auto text = refMan->makeUniqueRef<UIOTextEdit>();

		//auto test = refMan->makeUniqueRef<UIOScaling>(std::move(text));
		auto test2 = refMan->makeUniqueRef<UIOWindow>(std::move(text));
		test2.get()->screenRectangle.setWidth(0.4f);
		test2.get()->screenRectangle.setHeight(0.4f);

		auto test3 = refMan->makeUniqueRef<UIOFreeSize>(std::move(test2));

		test3.get()->updateSize(r);

		UIs.push_back(std::move(test3));
	}

	{
		auto textDisplay = refMan->makeUniqueRef<UIOTextDisplay>();
		textDisplay.get()->setText("tesg 123\n\n\n12323");

		auto t2 = refMan->makeUniqueRef<UIOWindow>(std::move(textDisplay));
		t2.get()->screenRectangle.setWidth(0.4f);
		t2.get()->screenRectangle.setHeight(0.4f);

		auto t3 = refMan->makeUniqueRef<UIOFreeSize>(std::move(t2));

		t3.get()->updateSize(r);

		UIs.push_back(std::move(t3));
	}
	// hotbar
	{
		//auto tile = refMan->makeUniqueRef<UIOWindowTile>();
		//for (int i = 0; i < 10; i++) {
		//	auto e = refMan->makeUniqueRef<UIOPad>(refMan->makeUniqueRef<UIOButton>());
		//	e.get()->bottom = { UIOSizeType::PX, 5 };
		//	e.get()->top = { UIOSizeType::PX, 5 };
		//	e.get()->left = { UIOSizeType::PX, 5 };
		//	e.get()->right = { UIOSizeType::PX, 5 };
		//	tile.get()->add(std::move(e));
		//}

		//auto sized = refMan->makeUniqueRef<UIOConstrainSize>(std::move(tile));
		//sized.get()->maybeHeight = { UIOSizeType::RELATIVE_WIDTH, 0.09f };
		//sized.get()->maybeWidth = { UIOSizeType::RELATIVE_WIDTH, 0.9f };
		//sized.get()->alignment = CONSTRAIN_ALIGNMENT::BOTTOM;

		//r.set({ -1.0f, -1.0f }, { 1.0f, 1.0f });
		////r.set({ -0.25f,-0.25f }, { 0.25f, 0.25f });
		//sized.get()->updateSize(r);

		//UIs.push_back(std::move(sized));
	}

	{
		auto hotbar = refMan->makeUniqueRef<UIOHotbar>();

		auto sized = refMan->makeUniqueRef<UIOConstrainSize>(std::move(hotbar));
		sized.get()->maybeHeight = { UIOSizeType::RELATIVE_WIDTH, 0.09f };
		sized.get()->maybeWidth = { UIOSizeType::RELATIVE_WIDTH, 0.9f };
		sized.get()->alignment = CONSTRAIN_ALIGNMENT::BOTTOM;

		r.set({ -1.0f, -1.0f }, { 1.0f, 1.0f });
		sized.get()->updateSize(r);

		UIs.push_back(std::move(sized));
	}


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
