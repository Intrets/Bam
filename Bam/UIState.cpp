#include "common.h"

#include "UIState.h"
#include "UIOHotbar.h"
#include "UIOConstrainSize.h"
#include "UIOWindow.h"
#include "UIOFreeSize.h"
#include "UIOTextDisplay.h"
#include "UIOInvisible.h"
#include "ControlState.h"
#include "GameState.h"
#include "State.h"
#include "UIOProxy.h"
#include "UIOActivityLinker.h"
#include "UIOActivitySelector.h"
#include "Option.h"
#include "UIOTextEdit2.h"
#include "UIOSimpleTextDisplay.h"
#include "UIOGrid.h"
#include "UIOList.h"
#include "UIOPad.h"
#include "UIOSizeType.h"
#include "UIOConstructButtons.h"
#include "Saver.h"
#include "Loader.h"
#include "UIOEmpty.h"
#include "Timer.h"
#include "Colors.h"

glm::vec2 UIState::getCursorPositionWorld() {
	return this->cursorWorld;
}

glm::vec2 UIState::getCursorPositionScreen() {
	return this->cursorScreen;
}

void UIState::run(State& state) {
	CallBackBindResult focussedResult = this->UIs.front().get()->runFocussedBinds(state);
	if (focussedResult & BIND_RESULT::CLOSE) {
		this->UIs.pop_front();
	}

	for (auto it = this->UIs.begin(); it != this->UIs.end();) {
		auto& UI = *it;
		CallBackBindResult uiResult = UI.get()->runGlobalBinds(state);
		if (uiResult & BIND_RESULT::CLOSE) {
			it = this->UIs.erase(it);
		}
		else if (uiResult & BIND_RESULT::FOCUS) {
			auto temp = std::move(UI);
			it = this->UIs.erase(it);
			this->UIs.push_front(std::move(temp));
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
	for (auto& UI : this->UIs) {
		UI.get()->updateSize(r);
	}
}

void UIState::updateCursor(GLFWwindow* window, glm::vec2 cam) {
	double x;
	double y;
	glfwGetCursorPos(window, &x, &y);

	int32_t frameSizeX, frameSizeY;
	glfwGetFramebufferSize(window, &frameSizeX, &frameSizeY);
	x = x / frameSizeX;
	y = y / frameSizeY;
	y = 1 - y;
	x = 2 * x - 1;
	y = 2 * y - 1;

	float ratio = frameSizeX / static_cast<float>(frameSizeY);
	glm::vec2 viewport(ratio, 1.0f);
	viewport *= Option<OPTION::CL_VIEWPORTSCALE, float>::getVal();

	this->cursorScreen = glm::vec2(x, y);
	this->cursorWorld = cam + this->cursorScreen * viewport;
}

void UIState::appendRenderInfo(GameState& gameState, RenderInfo& renderInfo) {
	int32_t depth = 0;
	for (auto& UI : this->UIs) {
		depth = UI.get()->addRenderInfo(gameState, renderInfo, depth);
	}
}

UIState::UIState() {
	auto refMan = Locator<ReferenceManager<UIOBase>>::get();

	ScreenRectangle r;
	r.set({ -1.0f, -1.0f }, { 1.0f, 1.0f });

	// Hotbar
	{
		auto hotbar = refMan->makeUniqueRef<UIOHotbar>();

		// selector
		hotbar.get()->tools[0] = refMan->makeUniqueRef<UIOActivitySelector>();

		// linker
		auto linker = refMan->makeUniqueRef<UIOActivityLinker>();
		hotbar.get()->tools[1] = std::move(linker);

		//
		auto sized = refMan->makeUniqueRef<UIOConstrainSize>(std::move(hotbar));
		sized.get()->maybeHeight = { UIOSizeType::RELATIVE_WIDTH, 0.05f };
		sized.get()->maybeWidth = { UIOSizeType::RELATIVE_WIDTH, 0.5f };
		sized.get()->alignment = UIOConstrainSize::ALIGNMENT::BOTTOM;

		sized.get()->updateSize(r);

		this->UIs.push_back(std::move(sized));
	}

	// text edit test window
	{
		auto text = constructTextEdit();

		auto test2 = refMan->makeUniqueRef<UIOWindow>(std::move(text), "text edit test");
		test2.get()->screenRectangle.setWidth(0.4f);
		test2.get()->screenRectangle.setHeight(0.4f);

		auto test3 = refMan->makeUniqueRef<UIOFreeSize>(std::move(test2));
		test3.get()->updateSize(r);

		this->UIs.push_back(std::move(test3));

		// ==============
		//auto text = refMan->makeUniqueRef<UIOTextEdit>();

		////auto test = refMan->makeUniqueRef<UIOScaling>(std::move(text));
		//auto test2 = refMan->makeUniqueRef<UIOWindow>(std::move(text));
		//test2.get()->screenRectangle.setWidth(0.4f);
		//test2.get()->screenRectangle.setHeight(0.4f);

		//auto test3 = refMan->makeUniqueRef<UIOFreeSize>(std::move(test2));

		//test3.get()->updateSize(r);

		//this->UIs.push_back(std::move(test3));
	}

	// text display test window

	// save/load and other stuff
	{
		auto list = refMan->makeUniqueRef<UIOList>(UIOList::DIRECTION::DOWN);
		{
			auto text = refMan->makeUniqueRef<UIOTextDisplay>();
			text.get()->text.cursorIndex = -1;
			text.get()->addGlobalBind(
				{ ControlState::CONTROLS::EVERY_TICK, static_cast<int32_t>(ControlState::CONTROLSTATE_PRESSED) },
				[](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult {
				auto self = static_cast<UIOTextDisplay*>(self_);
				self->text.setString("");
				for (auto& line : Locator<Timer>::ref().print()) {
					self->text.addLine(line);
				}
				return BIND_RESULT::CONTINUE;
			});

			auto t2 = refMan->makeUniqueRef<UIOConstrainSize>(std::move(text));
			t2.get()->maybeHeight = UIOSizeType(UIOSizeType::PX, 120);

			list.get()->addElement(std::move(t2));
		}
		{
			auto space = refMan->makeUniqueRef<UIOConstrainSize>(refMan->makeUniqueRef<UIOEmpty>());
			space.get()->maybeHeight = UIOSizeType(UIOSizeType::PX, 5);
			list.get()->addElement(std::move(space));
		}
		{
			auto text = constructTextEdit("test.save");
			auto textPtr = text.get();

			auto t2 = refMan->makeUniqueRef<UIOConstrainSize>(std::move(text));
			t2.get()->maybeHeight = UIOSizeType(UIOSizeType::PX, 20);

			list.get()->addElement(std::move(t2));

			auto [b1, b1ptr] = constructButtonWithText("save", 1);
			b1ptr->onPress = [textPtr](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult {
				if (textPtr->text.lines.size() == 0) {
					return BIND_RESULT::CONTINUE;
				}
				auto name = textPtr->text.lines.front();
				name.erase(name.end() - 1);

				Locator<Log>::ref().putLine("saving: " + name);

				Saver(name).saveGame(params.gameState);
				return BIND_RESULT::CONTINUE;
			};

			auto b12 = refMan->makeUniqueRef<UIOConstrainSize>(std::move(b1));
			b12.get()->maybeHeight = UIOSizeType(UIOSizeType::PX, 20);

			list.get()->addElement(std::move(b12));


			auto [b2, b2ptr] = constructButtonWithText("load", 1);

			b2ptr->onPress = [textPtr](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult {
				if (textPtr->text.lines.size() == 0) {
					return BIND_RESULT::CONTINUE;
				}
				auto name = textPtr->text.lines.front();
				name.erase(name.end() - 1);

				Locator<Log>::ref().putLine("loading: " + name);

				Loader(name).loadGame(params.gameState);
				return BIND_RESULT::CONTINUE;
			};

			auto b22 = refMan->makeUniqueRef<UIOConstrainSize>(std::move(b2));
			b22.get()->maybeHeight = UIOSizeType(UIOSizeType::PX, 20);

			list.get()->addElement(std::move(b22));
		}
		{
			auto space = refMan->makeUniqueRef<UIOConstrainSize>(refMan->makeUniqueRef<UIOEmpty>());
			space.get()->maybeHeight = UIOSizeType(UIOSizeType::PX, 5);
			list.get()->addElement(std::move(space));
		}
		{
			auto [testbutton, ptr] = constructButtonWithText("Debug Render", 1);
			ptr->color = Option<OPTION::GR_DEBUG, bool>::getVal() ? COLORS::GREEN : COLORS::RED;
			ptr->onPress = [](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult {
				Option<OPTION::GR_DEBUG, bool>::setVal(!Option<OPTION::GR_DEBUG, bool>::getVal());
				auto self = static_cast<UIOButton*>(self_);
				self->color = Option<OPTION::GR_DEBUG, bool>::getVal() ? COLORS::GREEN : COLORS::RED;
				return BIND_RESULT::CONTINUE;
			};

			auto b = refMan->makeUniqueRef<UIOConstrainSize>(std::move(testbutton));
			b.get()->maybeHeight = UIOSizeType(UIOSizeType::PX, 20);

			list.get()->addElement(std::move(b));
		}
		{
			auto [testbutton, ptr] = constructButtonWithText("Toggle Seperate Render Thread", 1);
			ptr->color = Option<OPTION::GR_RENDERTHREAD, bool>::getVal() ? COLORS::GREEN : COLORS::RED;
			ptr->onRelease = [](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult {
				Option<OPTION::GR_RENDERTHREAD, bool>::setVal(!Option<OPTION::GR_RENDERTHREAD, bool>::getVal());
				auto self = static_cast<UIOButton*>(self_);
				self->color = Option<OPTION::GR_RENDERTHREAD, bool>::getVal() ? COLORS::GREEN : COLORS::RED;
				return BIND_RESULT::CONTINUE;
			};

			auto b = refMan->makeUniqueRef<UIOConstrainSize>(std::move(testbutton));
			b.get()->maybeHeight = UIOSizeType(UIOSizeType::PX, 20);

			list.get()->addElement(std::move(b));
		}
		{
			auto space = refMan->makeUniqueRef<UIOConstrainSize>(refMan->makeUniqueRef<UIOEmpty>());
			space.get()->maybeHeight = UIOSizeType(UIOSizeType::PX, 5);
			list.get()->addElement(std::move(space));
		}
		{
			auto textDisplay = refMan->makeUniqueRef<UIOTextDisplay>();

			textDisplay.get()->addGlobalBind({ ControlState::CONTROLS::EVERY_TICK, static_cast<int32_t>(ControlState::CONTROLSTATE_PRESSED) }, [&](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult {
				auto self = static_cast<UIOTextDisplay*>(self_);
				auto newLines = Locator<Log>::ref().getLines();
				for (auto& newLine : newLines) {
					self->text.addLine(newLine);
				}
				if (newLines.size() != 0) {
					self->text.moveCursor(glm::ivec2(0, newLines.size()));
				}
				return BIND_RESULT::CONTINUE;
			});

			auto b = refMan->makeUniqueRef<UIOConstrainSize>(std::move(textDisplay));
			b.get()->maybeHeight = UIOSizeType(UIOSizeType::PX, 100);

			list.get()->addElement(std::move(b));
		}

		auto test2 = refMan->makeUniqueRef<UIOWindow>(std::move(list), "debug stuff");
		test2.get()->screenRectangle.setWidth(0.31f);
		test2.get()->screenRectangle.setHeight(1.7f);
		test2.get()->screenRectangle.translate({ -1.0f, 1.0f });

		auto test3 = refMan->makeUniqueRef<UIOFreeSize>(std::move(test2));

		test3.get()->updateSize(r);

		this->UIs.push_back(std::move(test3));


		//auto grid = refMan->makeUniqueRef<UIOGrid>(glm::ivec2(4, 2));
		//auto grid = refMan->makeUniqueRef<UIOList>(UIOList::DIRECTION::HORIZONTAL);

		//for (int32_t i = 0; i < 8; i++) {
		//	auto t = refMan->makeUniqueRef<UIOSimpleTextDisplay>(std::to_string(i));
		//	grid.get()->addElement(std::move(t));
		//}

		//auto test2 = refMan->makeUniqueRef<UIOWindow>(std::move(grid));
		//test2.get()->screenRectangle.setWidth(0.4f);
		//test2.get()->screenRectangle.setHeight(0.4f);
		//test2.get()->screenRectangle.translate({ 0.0f, -0.5f });

		//auto test3 = refMan->makeUniqueRef<UIOFreeSize>(std::move(test2));

		//test3.get()->updateSize(r);

		//this->UIs.push_back(std::move(test3));

	}

	// wasd movement in world
	{
		UniqueReference<UIOBase, UIOInvisible> movement = refMan->makeUniqueRef<UIOInvisible>();

		movement.get()->addGlobalBind({ ControlState::CONTROLS::TEST_LEFT, ControlState::CONTROLSTATE_PRESSED | ControlState::CONTROLSTATE_DOWN }, [&](UIOCallBackParams& state, UIOBase* self_) -> CallBackBindResult {
			state.player.pos.x -= 1.0f;
			return BIND_RESULT::CONTINUE;
		});

		movement.get()->addGlobalBind({ ControlState::CONTROLS::TEST_RIGHT, ControlState::CONTROLSTATE_PRESSED | ControlState::CONTROLSTATE_DOWN }, [&](UIOCallBackParams& state, UIOBase* self_) -> CallBackBindResult {
			state.player.pos.x += 1.0f;
			return BIND_RESULT::CONTINUE;
		});

		movement.get()->addGlobalBind({ ControlState::CONTROLS::TEST_DOWN, ControlState::CONTROLSTATE_PRESSED | ControlState::CONTROLSTATE_DOWN }, [&](UIOCallBackParams& state, UIOBase* self_) -> CallBackBindResult {
			state.player.pos.y -= 1.0f;
			return BIND_RESULT::CONTINUE;
		});

		movement.get()->addGlobalBind({ ControlState::CONTROLS::TEST_UP, ControlState::CONTROLSTATE_PRESSED | ControlState::CONTROLSTATE_DOWN }, [&](UIOCallBackParams& state, UIOBase* self_) -> CallBackBindResult {
			state.player.pos.y += 1.0f;
			return BIND_RESULT::CONTINUE;
		});

		this->UIs.push_back(std::move(movement));
	}

}
