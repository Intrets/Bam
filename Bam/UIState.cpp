#include "common.h"

#include "UIState.h"
#include "UIOHotbar.h"
#include "UIOConstrainSize.h"
#include "UIOTextEdit.h"
#include "UIOWindow.h"
#include "UIOFreeSize.h"
#include "UIOTextDisplay.h"
#include "UIOInvisible.h"
#include "ControlState.h"
#include "GameState.h"
#include "State.h"

void UIState::run(State& state) {
	CallBackBindResult focussedResult = UIs.front().get()->runFocussedBinds(state);
	if (focussedResult & BIND_RESULT::CLOSE) {
		UIs.pop_front();
	}

	for (auto it = UIs.begin(); it != UIs.end();) {
		auto& UI = *it;
		CallBackBindResult uiResult = UI.get()->runGlobalBinds(state);
		if (uiResult & BIND_RESULT::CLOSE) {
			it = UIs.erase(it);
		}
		else if (uiResult & BIND_RESULT::FOCUS) {
			auto temp = std::move(UI);
			it = UIs.erase(it);
			UIs.push_front(std::move(temp));
		}
		else {
			++it;
		}
	}
}

void UIState::updateSize(GLFWwindow* window) {
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

void UIState::appendRenderInfo(GameState& gameState, RenderInfo& renderInfo) {
	int32_t depth = 0;
	for (auto& UI : UIs) {
		depth = UI.get()->addRenderInfo(gameState, renderInfo, depth);
	}
}

UIState::UIState() {
	auto refMan = Locator<ReferenceManager<UIOBase>>::getService();

	ScreenRectangle r;
	r.set({ -1.0f, -1.0f }, { 1.0f, 1.0f });

	// Hotbar
	{
		auto hotbar = refMan->makeUniqueRef<UIOHotbar>();

		auto sized = refMan->makeUniqueRef<UIOConstrainSize>(std::move(hotbar));
		sized.get()->maybeHeight = { UIOSizeType::RELATIVE_WIDTH, 0.09f };
		sized.get()->maybeWidth = { UIOSizeType::RELATIVE_WIDTH, 0.9f };
		sized.get()->alignment = CONSTRAIN_ALIGNMENT::BOTTOM;

		sized.get()->updateSize(r);

		this->UIs.push_back(std::move(sized));
	}

	// text edit test window
	{
		auto text = refMan->makeUniqueRef<UIOTextEdit>();

		//auto test = refMan->makeUniqueRef<UIOScaling>(std::move(text));
		auto test2 = refMan->makeUniqueRef<UIOWindow>(std::move(text));
		test2.get()->screenRectangle.setWidth(0.4f);
		test2.get()->screenRectangle.setHeight(0.4f);

		auto test3 = refMan->makeUniqueRef<UIOFreeSize>(std::move(test2));

		test3.get()->updateSize(r);

		this->UIs.push_back(std::move(test3));
	}

	// text display test window
	{
		auto textDisplay = refMan->makeUniqueRef<UIOTextDisplay>();
		textDisplay.get()->setText("tesg 123\n\n\n12323");

		auto t2 = refMan->makeUniqueRef<UIOWindow>(std::move(textDisplay));
		t2.get()->screenRectangle.setWidth(0.4f);
		t2.get()->screenRectangle.setHeight(0.4f);

		auto t3 = refMan->makeUniqueRef<UIOFreeSize>(std::move(t2));

		t3.get()->updateSize(r);

		this->UIs.push_back(std::move(t3));
	}

	// Activity selector
	{
		UniqueReference<UIOBase, UIOActivitySelector> sel = refMan->makeUniqueRef<UIOActivitySelector>();
		this->selector = sel;
		this->UIs.push_back(std::move(sel));
	}

	// wasd movement in world
	{
		UniqueReference<UIOBase, UIOInvisible> movement = refMan->makeUniqueRef<UIOInvisible>();

		movement.get()->addGlobalBind({ CONTROLS::TEST_LEFT, CONTROLSTATE::CONTROLSTATE_PRESSED | CONTROLSTATE::CONTROLSTATE_DOWN }, [&](UIOCallBackParams& state, UIOBase* self_) -> CallBackBindResult {
			state.gameState.playerPos.x -= 1.0f;
			return BIND_RESULT::CONTINUE;
		});

		movement.get()->addGlobalBind({ CONTROLS::TEST_RIGHT, CONTROLSTATE::CONTROLSTATE_PRESSED | CONTROLSTATE::CONTROLSTATE_DOWN }, [&](UIOCallBackParams& state, UIOBase* self_) -> CallBackBindResult {
			state.gameState.playerPos.x += 1.0f;
			return BIND_RESULT::CONTINUE;
		});

		movement.get()->addGlobalBind({ CONTROLS::TEST_DOWN, CONTROLSTATE::CONTROLSTATE_PRESSED | CONTROLSTATE::CONTROLSTATE_DOWN }, [&](UIOCallBackParams& state, UIOBase* self_) -> CallBackBindResult {
			state.gameState.playerPos.y -= 1.0f;
			return BIND_RESULT::CONTINUE;
		});

		movement.get()->addGlobalBind({ CONTROLS::TEST_UP, CONTROLSTATE::CONTROLSTATE_PRESSED | CONTROLSTATE::CONTROLSTATE_DOWN }, [&](UIOCallBackParams& state, UIOBase* self_) -> CallBackBindResult {
			state.gameState.playerPos.y += 1.0f;
			return BIND_RESULT::CONTINUE;
		});

		this->UIs.push_back(std::move(movement));
	}
}
