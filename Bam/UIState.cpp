#include "common.h"

#include "UIState.h"
#include "UIOConstrainSize.h"
#include "UIOWindow.h"
#include "UIOInvisible.h"
#include "ControlState.h"
#include "GameState.h"
#include "PlayerState.h"
#include "Option.h"
#include "UIOEmpty.h"
#include "Timer.h"
#include "UIOInventory.h"
#include "UIOCursor.h"
#include "UIOHotbar.h"
#include "UIOConstructer2.h"
#include "UIOConstructDebugInfo.h"
#include "UIOConstructActivityBuilder.h"
#include "UIODropDownList.h"

CallBackBindResult UIState::runFrontBinds(PlayerState& state) {
	if (this->UIs.size() == 0) {
		return BIND::RESULT::CONTINUE;
	}

	CallBackBindResult activeResult =
		this->UIs.front().get()->runOnHoverBinds(state) |
		this->UIs.front().get()->runFocussedBinds(state) |
		this->UIs.front().get()->runActiveBinds(state) |
		this->UIs.front().get()->runGlobalBinds(state);
	CallBackBindResult res = 0;

	bool shouldExit = false;
	if (activeResult & BIND::RESULT::CLOSE) {
		this->UIs.pop_front();
		res |= BIND::RESULT::CLOSE;
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

void UIState::runUIBinds(PlayerState& state) {
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
		if (front & BIND::RESULT::CLOSE) {
			front &= ~BIND::RESULT::CLOSE;
		}
		else {
			++it;
		}
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

void UIState::run(PlayerState& state) {
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
		this->UIs.push_front(std::move(UI));
	}
	this->UIsBuffer.clear();

	for (auto& [name, ui] : this->namedUIsBuffer) {
		this->namedUIs.emplace(std::make_pair(name, ManagedReference<UIOBase, UIOBase>(ui)));
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
	ref.get()->addOnHoverBind({ CONTROL::KEY::MOUSE_POS_CHANGED_TOPLEVEL }, [](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
	{
		return BIND::RESULT::CONSUME;
	});
	this->UIsBuffer.push_back(std::move(ref));
}

bool UIState::addNamedUI(std::string const& name, std::function<UniqueReference<UIOBase, UIOBase>()> f) {
	auto namedUI = this->namedUIs.find(name);

	if (namedUI != this->namedUIs.end() && namedUI->second.isValid()) {
		if (auto ui = namedUI->second.getRef()) {
			for (auto it = this->UIs.begin(); it != this->UIs.end(); it++) {
				if (*it == ui) {
					this->UIsBuffer.push_back(std::move(*it));
					this->UIs.erase(it);
					break;
				}
			}
		}
		return false;
	}

	auto namedUIBuffered = this->namedUIsBuffer.find(name);

	if (namedUIBuffered != this->namedUIsBuffer.end()) {
		return false;
	}

	this->namedUIsBuffer[name] = f();
	this->namedUIsBuffer[name].get()->addOnHoverBind({ CONTROL::KEY::MOUSE_POS_CHANGED_TOPLEVEL }, [](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
	{
		return BIND::RESULT::CONSUME;
	});
	return true;
}

void UIState::addNamedUIReplace(std::string const& name, UniqueReference<UIOBase, UIOBase> ref) {
	this->closeNamedUI(name);

	ref.get()->addOnHoverBind({ CONTROL::KEY::MOUSE_POS_CHANGED_TOPLEVEL }, [](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
	{
		return BIND::RESULT::CONSUME;
	});

	this->namedUIsBuffer[name] = std::move(ref);
}

void UIState::closeNamedUI(std::string const& name) {
	auto namedUI = this->namedUIs.find(name);

	if (namedUI != this->namedUIs.end()) {
		if (auto ui = namedUI->second.getRef()) {
			for (auto it = this->UIs.begin(); it != this->UIs.end(); it++) {
				if (*it == ui) {
					this->closedBuffer.push_back(std::move(*it));
					this->UIs.erase(it);
					break;
				}
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

void UIState::closeUI(WeakReference<UIOBase, UIOBase> ref) {
	for (auto it = this->UIs.begin(); it != this->UIs.end(); it++) {
		if (*it == ref) {
			this->closedBuffer.push_back(std::move(*it));
			this->UIs.erase(it);
			break;
		}
	}
}

void UIState::reset() {
	this->shouldReset_ = true;
}

bool UIState::shouldReset() {
	return this->shouldReset_;
}

void UIState::init() {
	// Inventory
	{
		UIO2::Global::push();

		UIO2::hideable();
		UIO2::window("Inventory", { {0.5f - 0.04f, -0.1f - 0.04f}, {1.0f - 0.04f, 1.0f - 0.04f} },
					 UIOWindow::TYPE::MINIMISE |
					 UIOWindow::TYPE::MOVE |
					 UIOWindow::TYPE::HIDE);
		UIO2::makeEnd<UIOInventory>();

		this->addUI(UIO2::Global::pop());
	}

	// Cursor renderer
	{
		UIO2::Global::push();

		UIO2::makeEnd<UIOCursor>();

		this->addUI(UIO2::Global::pop());
	}

	// Hotbar
	{
		UIO2::Global::push();

		UIO2::constrainHeight({ UIO::SIZETYPE::RELATIVE_WIDTH, 0.05f });
		UIO2::constrainWidth({ UIO::SIZETYPE::RELATIVE_WIDTH, 0.5f });
		UIO2::alignBottom();
		UIO2::background(COLORS::UI::WINDOWBACKGROUND);
		UIO2::makeEnd<UIOHotbar>();

		this->addUI(UIO2::Global::pop());
	}

	// save/load and other stuff
	{
		UIO2::Global::push();

		UIO2::window("Debug Info", { {-1.0f, -0.8f}, {-0.7f, 1.0f} },
					 UIOWindow::TYPE::MINIMISE |
					 UIOWindow::TYPE::RESIZEVERTICAL |
					 UIOWindow::TYPE::RESIZEHORIZONTAL |
					 UIOWindow::TYPE::RESIZE |
					 UIOWindow::TYPE::MOVE);
		UIO2::constructDebugInfo();

		this->addUI(UIO2::Global::pop());
	}

	// wasd movement in world
	{
		UniqueReference<UIOBase, UIOInvisible> movement = UIO2::Global::getManager().makeUniqueRef<UIOInvisible>();

		movement.get()->addGlobalBind({ CONTROL::KEY::LEFT, CONTROL::STATE::PRESSED | CONTROL::STATE::DOWN }, [&](UIOCallBackParams& state, UIOBase* self_) -> CallBackBindResult
		{
			state.getPlayer().pos.x -= 1.0f;
			return BIND::RESULT::CONTINUE;
		});

		movement.get()->addGlobalBind({ CONTROL::KEY::RIGHT, CONTROL::STATE::PRESSED | CONTROL::STATE::DOWN }, [&](UIOCallBackParams& state, UIOBase* self_) -> CallBackBindResult
		{
			state.getPlayer().pos.x += 1.0f;
			return BIND::RESULT::CONTINUE;
		});

		movement.get()->addGlobalBind({ CONTROL::KEY::DOWN, CONTROL::STATE::PRESSED | CONTROL::STATE::DOWN }, [&](UIOCallBackParams& state, UIOBase* self_) -> CallBackBindResult
		{
			state.getPlayer().pos.y -= 1.0f;
			return BIND::RESULT::CONTINUE;
		});

		movement.get()->addGlobalBind({ CONTROL::KEY::UP, CONTROL::STATE::PRESSED | CONTROL::STATE::DOWN }, [&](UIOCallBackParams& state, UIOBase* self_) -> CallBackBindResult
		{
			state.getPlayer().pos.y += 1.0f;
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
