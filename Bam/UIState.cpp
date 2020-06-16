#include "common.h"

#include "UIState.h"
#include "UIOHotbar.h"
#include "UIOConstrainSize.h"
#include "UIOTextEdit.h"
#include "UIOWindow.h"
#include "UIOFreeSize.h"
#include "UIOTextDisplay.h"

void UIState::run(GameState& gameState, ControlState& controlState) {
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

	UniqueReference<UIOBase, UIOActivitySelector> sel = refMan->makeUniqueRef<UIOActivitySelector>();

	this->selector = sel;
	this->UIs.push_back(std::move(sel));

	ScreenRectangle r;
	r.set({ -1.0f, -1.0f }, { 1.0f, 1.0f });

	{
		auto hotbar = refMan->makeUniqueRef<UIOHotbar>();

		auto sized = refMan->makeUniqueRef<UIOConstrainSize>(std::move(hotbar));
		sized.get()->maybeHeight = { UIOSizeType::RELATIVE_WIDTH, 0.09f };
		sized.get()->maybeWidth = { UIOSizeType::RELATIVE_WIDTH, 0.9f };
		sized.get()->alignment = CONSTRAIN_ALIGNMENT::BOTTOM;

		sized.get()->updateSize(r);

		this->UIs.push_back(std::move(sized));
	}

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
}
