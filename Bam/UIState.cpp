#include "common.h"

#include "UIState.h"
#include "UIOConstrainSize.h"
#include "UIOWindow.h"
#include "UIOFreeSize.h"
#include "UIOTextDisplay.h"
#include "UIOInvisible.h"
#include "ControlState.h"
#include "GameState.h"
#include "State.h"
#include "Option.h"
#include "UIOTextConstructers.h"
#include "UIOGrid.h"
#include "UIOList.h"
#include "UIOPad.h"
#include "UIOSizeType.h"
#include "Saver.h"
#include "Loader.h"
#include "UIOEmpty.h"
#include "Timer.h"
#include "Colors.h"
#include "UIOColoredBackground.h"
#include "UIOConstructer.h"
#include "UIOBinds.h"
#include "StringHelpers.h"
#include "LuaActivity.h"
#include "Block.h"
#include "Incinerator.h"
#include "Forwarder.h"
#include "UIOInventory.h"
#include "UIOCursor.h"
#include "UIOHotbar.h"
#include "Inventory.h"
#include <fstream>
#include "ActivitySpawner.h"
#include "UIOConstructItemSpawner.h"
#include "UIOActivityInterface.h"
#include "UIOConstructer2.h"
#include "UIOConstructDebugInfo.h"

#include "UIOConstructDebugInfo.h"

CallBackBindResult UIState::runFrontBinds(State& state) {
	CallBackBindResult activeResult =
		this->UIs.front().get()->runOnHoverBinds(state) |
		this->UIs.front().get()->runFocussedBinds(state) |
		this->UIs.front().get()->runActiveBinds(state) |
		this->UIs.front().get()->runGlobalBinds(state);
	CallBackBindResult res = 0;

	bool shouldExit = false;
	if (activeResult & BIND::RESULT::CLOSE) {
		this->UIs.pop_front();
		shouldExit = true;
	}
	if (activeResult & BIND::RESULT::STOP) {
		res |= BIND::RESULT::STOP;
		shouldExit = true;
	}

	if (shouldExit) {
		return res;
	}

	return BIND::RESULT::CONTINUE;
}

glm::vec2 UIState::getCursorPositionWorld() {
	return this->cursorWorld;
}

glm::vec2 UIState::getCursorPositionScreen() {
	return this->cursorScreen;
}

glm::vec2 UIState::getCursorPositionScreenClamped(float c) {
	return glm::clamp(this->getCursorPositionScreen(), -c, c);
}

void UIState::runUIBinds(State& state) {
	auto front = this->runFrontBinds(state);

	if (front & BIND::RESULT::STOP) {
		return;
	}

	if (this->UIs.empty()) {
		return;
	}

	state.controlState.writeConsumedBuffer();

	if (this->UIs.size() > 1) {
		auto it = this->UIs.begin(), last = this->UIs.end();
		++it;
		for (; it != last;) {
			auto& UI = *it;
			CallBackBindResult res = UI.get()->runOnHoverBinds(state) | UI.get()->runGlobalBinds(state);

			if (res & BIND::RESULT::CLOSE) {
				it = this->UIs.erase(it);
			}
			else if (res & BIND::RESULT::FOCUS) {
				auto temp = std::move(UI);
				it = this->UIs.erase(it);
				this->UIs.push_front(std::move(temp));
			}
			else {
				it++;
			}
			if (res & BIND::RESULT::STOP) {
				return;
			}
			state.controlState.writeConsumedBuffer();
		}
	}

	if (!state.controlState.worldBindsBlocked()) {
		for (auto it = this->UIs.begin(); it != this->UIs.end();) {
			auto& UI = *it;
			CallBackBindResult res = UI.get()->runGameWorldBinds(state);

			if (res & BIND::RESULT::CLOSE) {
				it = this->UIs.erase(it);
			}
			else if (res & BIND::RESULT::FOCUS) {
				auto temp = std::move(UI);
				it = this->UIs.erase(it);
				this->UIs.push_front(std::move(temp));
			}
			else {
				it++;
			}
			if (res & BIND::RESULT::STOP) {
				return;
			}
			state.controlState.writeConsumedBuffer();
		}
	}
}

void UIState::run(State& state) {
	ScreenRectangle r;
	r.set({ -1.0f, -1.0f }, { 1.0f, 1.0f });

	this->runUIBinds(state);

	for (auto it = this->namedUIs.begin(), last = this->namedUIs.end(); it != last;) {
		if (!it->second.isValid()) {
			it = this->namedUIs.erase(it);
		}
		else {
			++it;
		}
	}

	for (auto& UI : this->UIsBuffer) {
		UI.get()->updateSize(r);
		this->UIs.push_front(std::move(UI));
	}
	this->UIsBuffer.clear();

	for (auto& [name, ui] : this->namedUIsBuffer) {
		this->namedUIs.emplace(std::make_pair(name, ui.handle));
		this->UIs.push_front(std::move(ui));
	}
	this->namedUIsBuffer.clear();

	this->closedBuffer.clear();
}

bool UIState::updateSize(GLFWwindow* window) {
	int32_t x;
	int32_t y;
	glfwGetWindowSize(window, &x, &y);

	if (x <= 0 || y <= 0) {
		return false;
	}

	ScreenRectangle r;
	r.setPixelSize(glm::ivec2(x, y));
	r.set({ -1,-1 }, { 1,1 });
	for (auto& UI : this->UIs) {
		UI.get()->updateSize(r);
	}
	return true;
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

	this->cursorScreen = glm::clamp(glm::vec2(x, y), -1.0f, 1.0f);
	this->cursorWorld = cam + this->cursorScreen * viewport;
}

void UIState::appendRenderInfo(GameState& gameState, RenderInfo& renderInfo) {
	int32_t depth = 10;
	for (auto& UI : this->UIs) {
		depth = UI.get()->addRenderInfo(gameState, renderInfo, depth);
	}
}

void UIState::addUI(UniqueReference<UIOBase, UIOBase> ref) {
	this->UIsBuffer.push_back(std::move(ref));
}

bool UIState::addNamedUI(std::string const& name, std::function<UniqueReference<UIOBase, UIOBase>()> f) {
	auto namedUI = this->namedUIs.find(name);

	if (namedUI != this->namedUIs.end() && namedUI->second.isValid()) {
		auto const& ui = namedUI->second;

		for (auto it = this->UIs.begin(); it != this->UIs.end(); it++) {
			if (it->handle == ui.getHandle()) {
				this->UIsBuffer.push_back(std::move(*it));
				this->UIs.erase(it);
				break;
			}
		}
		return false;
	}

	auto namedUIBuffered = this->namedUIsBuffer.find(name);

	if (namedUIBuffered != this->namedUIsBuffer.end()) {
		return false;
	}

	this->namedUIsBuffer[name] = f();
	return true;
}

void UIState::addNamedUIReplace(std::string const& name, std::function<UniqueReference<UIOBase, UIOBase>()> f) {
	this->closeNamedUI(name);

	this->namedUIsBuffer[name] = f();
}

void UIState::closeNamedUI(std::string const& name) {
	auto namedUI = this->namedUIs.find(name);

	if (namedUI != this->namedUIs.end()) {
		for (auto it = this->UIs.begin(); it != this->UIs.end(); it++) {
			if (it->handle == namedUI->second.getHandle()) {
				this->closedBuffer.push_back(std::move(*it));
				this->UIs.erase(it);
				break;
			}
		}
		this->namedUIs.erase(namedUI);
	}

	auto namedUIBuffered = this->namedUIsBuffer.find(name);

	if (namedUIBuffered != this->namedUIsBuffer.end()) {
		this->closedBuffer.push_back(std::move(namedUIBuffered->second));
		this->namedUIsBuffer.erase(namedUIBuffered);
	}
}

void UIState::reset() {
	this->shouldReset_ = true;
}

bool UIState::shouldReset() {
	return this->shouldReset_;
}

void UIState::init() {
	auto refMan = Locator<ReferenceManager<UIOBase>>::get();

	ScreenRectangle r;
	r.set({ -1.0f, -1.0f }, { 1.0f, 1.0f });

	// Inventory
	{
		UIO2::Global::start();

		UIO2::hideable();
		UIO2::window("Inventory", { {0.5f - 0.04f, -0.1f - 0.04f}, {1.0f - 0.04f, 1.0f - 0.04f} },
					 UIOWindow::TYPE::MINIMISE |
					 UIOWindow::TYPE::MOVE |
					 UIOWindow::TYPE::HIDE);
		UIO2::makeEnd<UIOInventory>();

		this->UIs.push_back(UIO2::Global::end());
	}

	// Cursor renderer
	{
		UIO2::Global::start();

		UIO2::makeEnd<UIOCursor>();

		this->UIs.push_back(UIO2::Global::end());
	}

	// Hotbar
	{
		UIO2::Global::start();

		UIO2::constrainHeight({ UIO::SIZETYPE::RELATIVE_WIDTH, 0.05f });
		UIO2::constrainWidth({ UIO::SIZETYPE::RELATIVE_WIDTH, 0.5f });
		UIO2::alignBottom();
		UIO2::background(COLORS::UI::WINDOWBACKGROUND);
		UIO2::makeEnd<UIOHotbar>();

		this->UIs.push_back(UIO2::Global::end());
	}

	// save/load and other stuff
	{
		UIO2::Global::start();

		UIO2::window("Debug Info", { {-1.0f, -0.8f}, {-0.7f, 1.0f} },
					 UIOWindow::TYPE::MINIMISE |
					 UIOWindow::TYPE::RESIZEVERTICAL |
					 UIOWindow::TYPE::RESIZEHORIZONTAL |
					 UIOWindow::TYPE::RESIZE |
					 UIOWindow::TYPE::MOVE);
		UIO2::constructDebugInfo();

		this->UIs.push_back(std::move(UIO2::Global::end()));
	}

	// wasd movement in world
	{
		UniqueReference<UIOBase, UIOInvisible> movement = refMan->makeUniqueRef<UIOInvisible>();

		movement.get()->addGlobalBind({ CONTROL::KEY::LEFT, CONTROL::STATE::PRESSED | CONTROL::STATE::DOWN }, [&](UIOCallBackParams& state, UIOBase* self_) -> CallBackBindResult
		{
			state.player.pos.x -= 1.0f;
			return BIND::RESULT::CONTINUE;
		});

		movement.get()->addGlobalBind({ CONTROL::KEY::RIGHT, CONTROL::STATE::PRESSED | CONTROL::STATE::DOWN }, [&](UIOCallBackParams& state, UIOBase* self_) -> CallBackBindResult
		{
			state.player.pos.x += 1.0f;
			return BIND::RESULT::CONTINUE;
		});

		movement.get()->addGlobalBind({ CONTROL::KEY::DOWN, CONTROL::STATE::PRESSED | CONTROL::STATE::DOWN }, [&](UIOCallBackParams& state, UIOBase* self_) -> CallBackBindResult
		{
			state.player.pos.y -= 1.0f;
			return BIND::RESULT::CONTINUE;
		});

		movement.get()->addGlobalBind({ CONTROL::KEY::UP, CONTROL::STATE::PRESSED | CONTROL::STATE::DOWN }, [&](UIOCallBackParams& state, UIOBase* self_) -> CallBackBindResult
		{
			state.player.pos.y += 1.0f;
			return BIND::RESULT::CONTINUE;
		});

		movement.get()->addGlobalBind({ CONTROL::KEY::SCROLL_UP }, [&](UIOCallBackParams& state, UIOBase* self_) -> CallBackBindResult
		{
			using viewport = Option<OPTION::CL_VIEWPORTSCALE, float>;
			viewport::setVal(viewport::getVal() * 0.8f);
			return BIND::RESULT::CONTINUE;
		});

		movement.get()->addGlobalBind({ CONTROL::KEY::SCROLL_DOWN }, [&](UIOCallBackParams& state, UIOBase* self_) -> CallBackBindResult
		{
			using viewport = Option<OPTION::CL_VIEWPORTSCALE, float>;
			viewport::setVal(viewport::getVal() / 0.8f);
			return BIND::RESULT::CONTINUE;
		});

		this->UIs.push_back(std::move(movement));
	}
}

void UIState::clear() {
	this->shouldReset_ = false;
	this->namedUIs.clear();
	this->UIs.clear();
	this->UIsBuffer.clear();
}

UIState::UIState() {
	this->init();
}
