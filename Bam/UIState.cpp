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
#include <fstream>

#include "UIOConstructActivityInterface.h"
#include "UIOActivityInterface.h"

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

	for (auto it = ++this->UIs.begin(); it != this->UIs.end();) {
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
	for (auto& UI : this->UIsBuffer) {
		UI.get()->updateSize(r);
		this->UIs.push_front(std::move(UI));
	}
	this->UIsBuffer.clear();
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
	int32_t depth = 0;
	for (auto& UI : this->UIs) {
		depth = UI.get()->addRenderInfo(gameState, renderInfo, depth);
	}
}

void UIState::addUI(UniqueReference<UIOBase, UIOBase> ref) {
	this->UIsBuffer.push_back(std::move(ref));
}

bool UIState::addNamedUI(std::string name, std::function<UniqueReference<UIOBase, UIOBase>()> f) {
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
	else {
		auto ui = f();
		this->namedUIs[name] = ManagedReference<UIOBase, UIOBase>(ui);
		this->addUI(std::move(ui));
		return true;
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

	UIOActivityInterface* interfacePtr;
	UIOHideable* interfaceHideablePtr;

	// Interface
	{
		this->UIs.push_back(
			CONSTRUCTER::constructActivityInteractor(interfacePtr)
			.window("Interactor", { {0.5f - 0.04f, -0.1f - 0.04f}, {1.0f - 0.04f, 1.0f - 0.04f} },
					UIOWindow::TYPE::MINIMISE |
					UIOWindow::TYPE::MOVE |
					UIOWindow::TYPE::HIDE)
			.hideable()
			.setPtr(interfaceHideablePtr)
			.get()
		);
	}

	// Hotbar
	{
		UIOHotbar* hotbarPtr;
		auto hotbar = UIOConstructer<UIOHotbar>::makeConstructer()
			.setPtr(hotbarPtr)
			.constrainHeight({ UIO::SIZETYPE::RELATIVE_WIDTH, 0.05f })
			.constrainWidth({ UIO::SIZETYPE::RELATIVE_WIDTH, 0.5f })
			.align(UIO::ALIGNMENT::BOTTOM)
			.get();

		hotbarPtr->setTool(0, "Piston", [interfacePtr, interfaceHideablePtr](UIOCallBackParams& params)
		{
			interfaceHideablePtr->show();
			interfacePtr->spawnHover(params.gameState, params.uiState.getCursorPositionWorld(), ACTIVITY::TYPE::PISTON);
			return BIND::RESULT::CONTINUE;
		});

		hotbarPtr->setTool(1, "Platform", [interfacePtr, interfaceHideablePtr](UIOCallBackParams& params)
		{
			interfaceHideablePtr->show();
			interfacePtr->spawnHover(params.gameState, params.uiState.getCursorPositionWorld(), ACTIVITY::TYPE::PLATFORM);
			return BIND::RESULT::CONTINUE;
		});

		hotbarPtr->setTool(2, "Crane", [interfacePtr, interfaceHideablePtr](UIOCallBackParams& params)
		{
			interfaceHideablePtr->show();
			interfacePtr->spawnHover(params.gameState, params.uiState.getCursorPositionWorld(), ACTIVITY::TYPE::RAILCRANE);
			return BIND::RESULT::CONTINUE;
		});

		hotbarPtr->setTool(3, "LUA", [interfacePtr, interfaceHideablePtr](UIOCallBackParams& params)
		{
			interfaceHideablePtr->show();
			interfacePtr->spawnHover(params.gameState, params.uiState.getCursorPositionWorld(), ACTIVITY::TYPE::LUA);
			return BIND::RESULT::CONTINUE;
		});

		hotbarPtr->setTool(4, "Grabber", [interfacePtr, interfaceHideablePtr](UIOCallBackParams& params)
		{
			interfaceHideablePtr->show();
			interfacePtr->spawnHover(params.gameState, params.uiState.getCursorPositionWorld(), ACTIVITY::TYPE::GRABBER);
			return BIND::RESULT::CONTINUE;
		});

		this->UIs.push_back(std::move(hotbar));
	}

	// save/load and other stuff
	{
		UIOList* listPtr;
		auto window = UIOConstructer<UIOList>::makeConstructer(UIO::DIR::DOWN)
			.setPtr(listPtr)
			.window("Debug Info", { {-1.0f, -0.8f}, {-0.7f, 1.0f} },
					UIOWindow::TYPE::MINIMISE |
					UIOWindow::TYPE::RESIZEVERTICAL |
					UIOWindow::TYPE::RESIZEHORIZONTAL |
					UIOWindow::TYPE::RESIZE |
					UIOWindow::TYPE::MOVE)
			.get();

		this->UIs.push_back(std::move(window));

		//auto list = refMan->makeUniqueRef<UIOList>(UIO::DIR::DOWN);
		{
			UIOTextDisplay* ptr;
			auto text = TextConstructer::constructDisplayText("").setPtr(ptr)
				.addBind([](UIOTextDisplay* ptr)
			{
				ptr->addGlobalBind(
					{ CONTROL::KEY::EVERY_TICK, static_cast<int32_t>(CONTROL::STATE::PRESSED) },
					[](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
				{
					auto self = static_cast<UIOTextDisplay*>(self_);
					self->setText(Locator<Timer>::ref().print());
					return BIND::RESULT::CONTINUE;
				});
			})
				.background(COLORS::UI::BACKGROUND)
				.constrainHeight({ UIO::SIZETYPE::FH, 16.3f })
				.get();

			listPtr->addElement(std::move(text));
		}
		{
			listPtr->addElement(
				UIOConstructer<UIOEmpty>::makeConstructer()
				.constrainHeight({ UIO::SIZETYPE::STATIC_PX, 4 })
				.get()
			);
		}
		{
			UIOTextDisplay* textPtr;
			auto text = TextConstructer::constructSingleLineTextEdit("test.save").setPtr(textPtr)
				.background(COLORS::UI::BACKGROUND)
				.constrainHeight({ UIO::SIZETYPE::FH, 1.2f })
				.get();

			listPtr->addElement(std::move(text));

			auto saveButton = TextConstructer::constructSingleLineDisplayText("save")
				.align(UIO::ALIGNMENT::CENTER)
				.button()
				.onPress([textPtr](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
			{
				if (textPtr->text.getLines().size() == 0) {
					return BIND::RESULT::CONTINUE;
				}
				auto name = textPtr->text.getLines().front();
				name.erase(name.end() - 1);

				params.gameState.saveFile = name;
				return BIND::RESULT::CONTINUE;
			})
				.pad({ UIO::SIZETYPE::STATIC_PX, 1 })
				.constrainHeight({ UIO::SIZETYPE::FH, 1.2f })
				.get();

			listPtr->addElement(std::move(saveButton));

			auto loadButton = TextConstructer::constructSingleLineDisplayText("load")
				.align(UIO::ALIGNMENT::CENTER)
				.button()
				.onPress([textPtr](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
			{
				if (textPtr->text.getLines().size() == 0) {
					return BIND::RESULT::CONTINUE;
				}
				auto name = textPtr->text.getLines().front();
				name.erase(name.end() - 1);

				params.gameState.loadFile = name;
				params.uiState.reset();
				return BIND::RESULT::CONTINUE;
			})
				.pad({ UIO::SIZETYPE::STATIC_PX, 1 })
				.constrainHeight({ UIO::SIZETYPE::FH, 1.2f })
				.get();

			listPtr->addElement(std::move(loadButton));
		}
		{
			listPtr->addElement(
				UIOConstructer<UIOEmpty>::makeConstructer()
				.constrainHeight({ UIO::SIZETYPE::STATIC_PX, 4 })
				.get()
			);
		}
		{
			UIOButton* ptr;
			auto a = TextConstructer::constructSingleLineDisplayText("Debug Render")
				.align(UIO::ALIGNMENT::CENTER)
				.button()
				.setPtr(ptr)
				.onRelease(
					[](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
			{
				Option<OPTION::GR_DEBUG, bool>::setVal(!Option<OPTION::GR_DEBUG, bool>::getVal());
				auto self = static_cast<UIOButton*>(self_);
				self->setColor(Option<OPTION::GR_DEBUG, bool>::getVal() ? COLORS::UI::GREEN : COLORS::UI::RED);
				return BIND::RESULT::CONTINUE;
			})
				.pad({ UIO::SIZETYPE::STATIC_PX, 1 })
				.constrainHeight({ UIO::SIZETYPE::FH, 1.2f })
				.get();

			ptr->setColor(Option<OPTION::GR_DEBUG, bool>::getVal() ? COLORS::UI::GREEN : COLORS::UI::RED);

			listPtr->addElement(std::move(a));
		}
		{
			UIOButton* ptr;
			auto a = TextConstructer::constructSingleLineDisplayText("Toggle Seperate Render Thread")
				.align(UIO::ALIGNMENT::CENTER)
				.button()
				.setPtr(ptr)
				.onRelease(
					[](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
			{
				Option<OPTION::GR_RENDERTHREAD, bool>::setVal(!Option<OPTION::GR_RENDERTHREAD, bool>::getVal());
				auto self = static_cast<UIOButton*>(self_);
				self->setColor(Option<OPTION::GR_RENDERTHREAD, bool>::getVal() ? COLORS::UI::GREEN : COLORS::UI::RED);
				return BIND::RESULT::CONTINUE;
			})
				.pad({ UIO::SIZETYPE::STATIC_PX, 1 })
				.constrainHeight({ UIO::SIZETYPE::FH, 1.2f })
				.get();

			ptr->setColor(Option<OPTION::GR_RENDERTHREAD, bool>::getVal() ? COLORS::UI::GREEN : COLORS::UI::RED);

			listPtr->addElement(std::move(a));
		}
		{
			listPtr->addElement(
				UIOConstructer<UIOEmpty>::makeConstructer()
				.constrainHeight({ UIO::SIZETYPE::STATIC_PX, 4 })
				.get()
			);
		}
		{
			auto a = TextConstructer::constructDisplayText("")
				.addBind(UIOBinds::Base::activatable)
				.addBind([](UIOTextDisplay* ptr)
			{
				ptr->addGlobalBind({ CONTROL::KEY::EVERY_TICK, static_cast<int32_t>(CONTROL::STATE::PRESSED) }, [](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
				{
					auto self = static_cast<UIOTextDisplay*>(self_);

					auto& vec = self->text.getLinesMutable();
					if (vec.size() > 100) {
						vec.erase(vec.begin(), vec.begin() + 50);
					}

					auto newLines = Locator<Log>::ref().getLines();
					for (auto& newLine : newLines) {
						self->text.addLine(newLine);
					}
					if (newLines.size() != 0) {
						self->text.moveCursor(glm::ivec2(100000, newLines.size()));
					}
					return BIND::RESULT::CONTINUE;
				});

			})
				.background(COLORS::UI::BACKGROUND)
				.get();

			listPtr->addElement(std::move(a));
		}
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
