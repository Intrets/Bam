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
#include "UIOTextConstructers.h"
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
#include "UIOColoredBackground.h"
#include "UIOConstructer.h"
#include "UIOBinds.h"
#include "StringHelpers.h"

#include "ActivityLuaTest.h"
#include "UIOActivityLuaTest.h"
#include <fstream>

glm::vec2 UIState::getCursorPositionWorld() {
	return this->cursorWorld;
}

glm::vec2 UIState::getCursorPositionScreen() {
	return this->cursorScreen;
}

void UIState::run(State& state) {
	for (auto it = this->UIs.begin(); it != this->UIs.end();) {
		auto& UI = *it;
		CallBackBindResult clickedResult = UI.get()->runOnHoverBinds(state);
		if (clickedResult & BIND_RESULT::CLOSE) {
			it = this->UIs.erase(it);
		}
		else if (clickedResult & BIND_RESULT::FOCUS) {
			auto temp = std::move(UI);
			it = this->UIs.erase(it);
			this->UIs.push_front(std::move(temp));
		}
		else {
			++it;
		}
		if (clickedResult & BIND_RESULT::STOP) {
			return;
		}
	}

	CallBackBindResult focussedResult = this->UIs.front().get()->runFocussedBinds(state);
	if (focussedResult & BIND_RESULT::CLOSE) {
		this->UIs.pop_front();
	}
	if (focussedResult & BIND_RESULT::STOP) {
		return;
	}

	// FEATURE: decide if active binds should be run for all UIs and not just the one in front 
	CallBackBindResult activeResult = this->UIs.front().get()->runActiveBinds(state);
	if (activeResult & BIND_RESULT::CLOSE) {
		this->UIs.pop_front();
	}
	if (activeResult & BIND_RESULT::STOP) {
		return;
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
		if (uiResult & BIND_RESULT::STOP) {
			return;
		}
	}
}

bool UIState::updateSize(GLFWwindow* window) {
	int32_t x;
	int32_t y;
	glfwGetWindowSize(window, &x, &y);

	if (x <= 0 || y <= 0) {
		return false;
	}

	ScreenRectangle r;
	r.screenPixels = glm::ivec2(x, y);
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
		UIOHotbar* hotbarPtr;
		auto hotbar = UIOConstructer<UIOHotbar>::makeConstructer()
			.setPtr(hotbarPtr)
			.constrainHeight(UIOSizeType(UIOSizeType::RELATIVE_WIDTH, 0.05f))
			.constrainWidth(UIOSizeType(UIOSizeType::RELATIVE_WIDTH, 0.5f))
			.align(UIOConstrainSize::ALIGNMENT::BOTTOM)
			.get();

		// selector
		hotbarPtr->tools[0] = refMan->makeUniqueRef<UIOActivitySelector>();

		// linker
		auto linker = refMan->makeUniqueRef<UIOActivityLinker>();
		hotbarPtr->tools[1] = std::move(linker);

		this->UIs.push_back(std::move(hotbar));

	}

	// save/load and other stuff
	{
		UIOList* listPtr;
		auto window = UIOConstructer<UIOList>::makeConstructer(UIOList::DIRECTION::DOWN)
			.setPtr(listPtr)
			.window("Debug Info", { {-1.0f, -0.8f}, {-0.7f, 1.0f} },
					UIOWindow::TYPE::MINIMISE |
					UIOWindow::TYPE::RESIZEVERTICAL)
			.get();

		this->UIs.push_back(std::move(window));

		//auto list = refMan->makeUniqueRef<UIOList>(UIOList::DIRECTION::DOWN);
		{
			UIOTextDisplay* ptr;
			auto text = TextConstructer::constructDisplayText("").setPtr(ptr)
				.addBind([](UIOTextDisplay* ptr)
			{
				ptr->addGlobalBind(
					{ ControlState::CONTROLS::EVERY_TICK, static_cast<int32_t>(ControlState::CONTROLSTATE_PRESSED) },
					[](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
				{
					auto self = static_cast<UIOTextDisplay*>(self_);
					self->setText(Locator<Timer>::ref().print());
					return BIND_RESULT::CONTINUE;
				});
			})
				.background(COLORS::BACKGROUND)
				.constrainHeight(UIOSizeType(UIOSizeType::PX, 120))
				.get();

			listPtr->addElement(std::move(text));
		}
		{
			listPtr->addElement(
				UIOConstructer<UIOEmpty>::makeConstructer()
				.constrainHeight(UIOSizeType(UIOSizeType::PX, 5))
				.get()
			);
		}
		{
			UIOTextDisplay* textPtr;
			auto text = TextConstructer::constructSingleLineTextEdit("test.save").setPtr(textPtr)
				.background(COLORS::BACKGROUND)
				.constrainHeight(UIOSizeType(UIOSizeType::PX, 20))
				.get();

			listPtr->addElement(std::move(text));

			auto saveButton = UIOConstructer<UIOSimpleTextDisplay>::makeConstructer("save")
				.align(UIOConstrainSize::ALIGNMENT::CENTER)
				.button()
				.onPress([textPtr](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
			{
				if (textPtr->text.lines.size() == 0) {
					return BIND_RESULT::CONTINUE;
				}
				auto name = textPtr->text.lines.front();
				name.erase(name.end() - 1);

				Locator<Log>::ref().putLine("saving: " + name);

				Saver(name).saveGame(params.gameState);
				return BIND_RESULT::CONTINUE;
			})
				.pad(UIOSizeType(UIOSizeType::PX, 1))
				.constrainHeight(UIOSizeType(UIOSizeType::PX, 20))
				.get();

			listPtr->addElement(std::move(saveButton));

			auto loadButton = UIOConstructer<UIOSimpleTextDisplay>::makeConstructer("load")
				.align(UIOConstrainSize::ALIGNMENT::CENTER)
				.button()
				.onPress([textPtr](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
			{
				if (textPtr->text.lines.size() == 0) {
					return BIND_RESULT::CONTINUE;
				}
				auto name = textPtr->text.lines.front();
				name.erase(name.end() - 1);

				Locator<Log>::ref().putLine("loading: " + name);

				Loader(name).loadGame(params.gameState);
				return BIND_RESULT::CONTINUE;
			})
				.pad(UIOSizeType(UIOSizeType::PX, 1))
				.constrainHeight(UIOSizeType(UIOSizeType::PX, 20))
				.get();

			listPtr->addElement(std::move(loadButton));
		}
		{
			listPtr->addElement(
				UIOConstructer<UIOEmpty>::makeConstructer()
				.constrainHeight(UIOSizeType(UIOSizeType::PX, 5))
				.get()
			);
		}
		{
			UIOButton* ptr;
			auto a = TextConstructer::constructSingleLineDisplayText("Debug Render")
				.align(UIOConstrainSize::ALIGNMENT::CENTER)
				.button()
				.setPtr(ptr)
				.onRelease(
					[](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
			{
				Option<OPTION::GR_DEBUG, bool>::setVal(!Option<OPTION::GR_DEBUG, bool>::getVal());
				auto self = static_cast<UIOButton*>(self_);
				self->color = Option<OPTION::GR_DEBUG, bool>::getVal() ? COLORS::GREEN : COLORS::RED;
				return BIND_RESULT::CONTINUE;
			})
				.pad(UIOSizeType(UIOSizeType::PX, 1))
				.constrainHeight(UIOSizeType(UIOSizeType::PX, 20))
				.get();

			ptr->color = Option<OPTION::GR_DEBUG, bool>::getVal() ? COLORS::GREEN : COLORS::RED;

			listPtr->addElement(std::move(a));
		}
		{
			UIOButton* ptr;
			auto a = TextConstructer::constructSingleLineDisplayText("Toggle Seperate Render Thread")
				.align(UIOConstrainSize::ALIGNMENT::CENTER)
				.button()
				.setPtr(ptr)
				.onRelease(
					[](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
			{
				Option<OPTION::GR_RENDERTHREAD, bool>::setVal(!Option<OPTION::GR_RENDERTHREAD, bool>::getVal());
				auto self = static_cast<UIOButton*>(self_);
				self->color = Option<OPTION::GR_RENDERTHREAD, bool>::getVal() ? COLORS::GREEN : COLORS::RED;
				return BIND_RESULT::CONTINUE;
			})
				.pad(UIOSizeType(UIOSizeType::PX, 1))
				.constrainHeight(UIOSizeType(UIOSizeType::PX, 20))
				.get();

			ptr->color = Option<OPTION::GR_RENDERTHREAD, bool>::getVal() ? COLORS::GREEN : COLORS::RED;

			listPtr->addElement(std::move(a));
		}
		{
			listPtr->addElement(
				UIOConstructer<UIOEmpty>::makeConstructer()
				.constrainHeight(UIOSizeType(UIOSizeType::PX, 5))
				.get()
			);
		}
		{
			auto a = TextConstructer::constructDisplayText("")
				.addBaseBind(UIOBinds::Base::activatable)
				.addBind([](UIOTextDisplay* ptr)
			{
				ptr->addGlobalBind({ ControlState::CONTROLS::EVERY_TICK, static_cast<int32_t>(ControlState::CONTROLSTATE_PRESSED) }, [](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
				{
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

			})
				.background(COLORS::BACKGROUND)
				.get();

			listPtr->addElement(std::move(a));
		}
	}

	// lua test window
	{
		UIOList* listPtr;
		auto window = UIOConstructer<UIOList>::makeConstructer(UIOList::DIRECTION::DOWN)
			.setPtr(listPtr)
			.window("LUA test", { {0.5f, -0.8f}, {1.0f, 1.0f} },
					UIOWindow::TYPE::MINIMISE |
					UIOWindow::TYPE::RESIZEVERTICAL)
			.get();

		this->UIs.push_back(std::move(window));

		auto luaTest = UIOConstructer<UIOActivityLuaTest>::makeConstructer().get();
		auto luaTestPtr = luaTest.get();

		listPtr->addElement(std::move(luaTest));

		{
			UIOGrid* dirsPtr;
			auto dirs = UIOConstructer<UIOGrid>::makeConstructer(glm::ivec2(5, 1))
				.setPtr(dirsPtr)
				.constrainHeight(UIOSizeType(UIOSizeType::PX, 20))
				.align(UIOConstrainSize::ALIGNMENT::TOP)
				.get();

			{
				UIOTextDisplay* textPtr;

				auto text = TextConstructer::constructSingleLineTextEdit("1")
					.setPtr(textPtr)
					.background(COLORS::BACKGROUND)
					.constrainHeight(UIOSizeType(UIOSizeType::PX, 20))
					.get();

				dirsPtr->addElement(std::move(text));

				for (auto [buttonName, dir_] : {
					std::make_tuple("Left", Activity::DIR::LEFT),
					std::make_tuple("Right", Activity::DIR::RIGHT),
					std::make_tuple("Up", Activity::DIR::UP),
					std::make_tuple("Down", Activity::DIR::DOWN),
					 }) {
					Activity::DIR dir = dir_;
					UIOButton* ptr;
					auto a = TextConstructer::constructSingleLineDisplayText(buttonName)
						.align(UIOConstrainSize::ALIGNMENT::CENTER)
						.button()
						.setPtr(ptr)
						.onRelease(
							[dir, textPtr](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
					{
						std::string in = textPtr->text.lines.front();
						int n;
						try {
							n = std::stoi(in);
							n = glm::clamp(n, 1, 50);
						}
						catch (...) {
							n = 10;
						}

						if (params.player.selection.target.isValid()) {
							bool success = params.player.selection.target.get()->applyMoveRoot(params.gameState, dir, n);
							if (success) {
								Locator<Log>::ref() << Log::OPEN{} << "moving with pace " << n << "\n" << Log::CLOSE{};
							}
							return BIND_RESULT::CONTINUE;

						}
						else {
							return BIND_RESULT::CONTINUE;
						}
					})
						.pad(UIOSizeType(UIOSizeType::PX, 1))
						.constrainHeight(UIOSizeType(UIOSizeType::PX, 20))
						.get();

					dirsPtr->addElement(std::move(a));
				}
			}

			listPtr->addElement(std::move(dirs));
		}

		UIOTextDisplay* luaTextPtr;
		{
			auto luaEditor = TextConstructer::constructTextEdit("test(h ,1)")
				.setPtr(luaTextPtr)
				.background(COLORS::BACKGROUND)
				.constrainHeight(UIOSizeType(UIOSizeType::RELATIVE_HEIGHT, 0.5f))
				.get();

			listPtr->addElement(std::move(luaEditor));
		}

		// list of vars to be watched
		UIOTextDisplay* watchTextPtr;
		// text window to display watched vars
		UIOTextDisplay* displayWatchTextPtr;
		UIOTextDisplay* outputTextPtr;
		// name of .lua file
		UIOTextDisplay* fileTextPtr;

		{
			UIOGrid* watchGridPtr;
			auto watchGrid = UIOConstructer<UIOGrid>::makeConstructer(glm::ivec2(2, 1))
				.setPtr(watchGridPtr)
				.constrainHeight(UIOSizeType(UIOSizeType::RELATIVE_HEIGHT, 0.9f))
				.get();

			{
				watchGridPtr->addElement(
					TextConstructer::constructTextEdit("")
					.setPtr(watchTextPtr)
					.background(COLORS::BACKGROUND)
					.pad(UIOSizeType(UIOSizeType::PX, 1))
					.get()
				);

				UIOGrid* outputGridPtr;

				watchGridPtr->addElement(
					UIOConstructer<UIOGrid>::makeConstructer(glm::ivec2(1, 2))
					.setPtr(outputGridPtr)
					.get()
				);

				outputGridPtr->addElement(
					TextConstructer::constructDisplayText("watch")
					.setPtr(displayWatchTextPtr)
					.background(COLORS::BACKGROUND)
					.pad(UIOSizeType(UIOSizeType::PX, 1))
					.get()
				);

				outputGridPtr->addElement(
					TextConstructer::constructDisplayText("output")
					.setPtr(outputTextPtr)
					.background(COLORS::BACKGROUND)
					.pad(UIOSizeType(UIOSizeType::PX, 1))
					.get()
				);
			}

			listPtr->addElement(std::move(watchGrid));
		}

		{
			UIOList* luaControlPtr;
			auto luaControl = UIOConstructer<UIOList>::makeConstructer(UIOList::DIRECTION::LEFT)
				.setPtr(luaControlPtr)
				.padTop(UIOSizeType(UIOSizeType::PX, 1))
				.constrainHeight(UIOSizeType(UIOSizeType::PX, 20))
				.get();

			auto fileText =
				TextConstructer::constructSingleLineTextEdit("test.lua")
				.setPtr(fileTextPtr)
				.background(COLORS::BACKGROUND)
				.get();

			listPtr->addElement(std::move(luaControl));

			{
				// Load
				luaControlPtr->addElement(
					TextConstructer::constructSingleLineDisplayText("Load")
					.align(UIOConstrainSize::ALIGNMENT::CENTER)
					.button()
					.onRelease([fileTextPtr, luaTextPtr](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
				{
					std::string name = fileTextPtr->text.lines.front();
					name.resize(name.size() - 1);

					std::ifstream file;
					Locator<PathManager>::ref().openLUA(file, name);

					luaTextPtr->setText("");

					std::string line;
					while (std::getline(file, line)) {
						luaTextPtr->text.addLine(line);
					}

					return BIND_RESULT::CONTINUE;
				})
					.pad(UIOSizeType(UIOSizeType::PX, 1))
					.constrainWidth(UIOSizeType(UIOSizeType::PX, 60))
					.get());

				// Save
				luaControlPtr->addElement(
					TextConstructer::constructSingleLineDisplayText("Save")
					.align(UIOConstrainSize::ALIGNMENT::CENTER)
					.button()
					.onRelease([fileTextPtr, luaTextPtr](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
				{
					std::string name = fileTextPtr->text.lines.front();
					name.resize(name.size() - 1);

					std::ofstream file;

					Locator<PathManager>::ref().openLUA(file, name);

					for (auto& line : luaTextPtr->text.lines) {
						file << line;
					}

					file.close();

					return BIND_RESULT::CONTINUE;
				})
					.pad(UIOSizeType(UIOSizeType::PX, 1))
					.constrainWidth(UIOSizeType(UIOSizeType::PX, 60))
					.get());

				// Run
				luaControlPtr->addElement(
					TextConstructer::constructSingleLineDisplayText("Run")
					.align(UIOConstrainSize::ALIGNMENT::CENTER)
					.button()
					.onRelease([luaTextPtr, luaTestPtr](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
				{
					luaTestPtr->lua.runScript(params.gameState, params.player.selection.target.handle);
					luaTestPtr->lua.state.script(join(luaTextPtr->text.lines));
					return BIND_RESULT::CONTINUE;
				})
					.pad(UIOSizeType(UIOSizeType::PX, 1))
					.constrainWidth(UIOSizeType(UIOSizeType::PX, 60))
					.get());

				// Read Vars
				luaControlPtr->addElement(
					TextConstructer::constructSingleLineDisplayText("Vars")
					.align(UIOConstrainSize::ALIGNMENT::CENTER)
					.button()
					.addGlobalBind({ ControlState::CONTROLS::EVERY_TICK, ControlState::CONTROLSTATE_PRESSED }, [luaTestPtr, watchTextPtr, displayWatchTextPtr](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
				{
					displayWatchTextPtr->text.setLines({ "Watched variables: ", "" });
					for (auto line : watchTextPtr->text.lines) {
						if (line.size() < 2) {
							return BIND_RESULT::CONTINUE;
						}
						line.resize(line.size() - 1);

						sol::object object = luaTestPtr->lua.state[line];
						auto type = object.get_type();
						std::string out = "invalid";

						switch (type) {
							case sol::type::none:
								break;
							case sol::type::lua_nil:
								break;
							case sol::type::string:
								out = object.as<std::string>();
								break;
							case sol::type::number:
								if (object.is<int>()) {
									out = std::to_string(object.as<int32_t>());
								}
								else {
									out = std::to_string(object.as<double>());
								}
								break;
							case sol::type::thread:
								break;
							case sol::type::boolean:
								out = object.as<bool>() ? "true" : "false";
								break;
							case sol::type::function:
								break;
							case sol::type::userdata:
								break;
							case sol::type::lightuserdata:
								break;
							case sol::type::table:
								break;
							case sol::type::poly:
								break;
							default:
								break;
						}

						displayWatchTextPtr->text.addLine(line + ": " + out);
					}
					displayWatchTextPtr->text.moveCursor(glm::ivec2(0, watchTextPtr->text.lines.size() + 1));

					return BIND_RESULT::CONTINUE;
				})
					.pad(UIOSizeType(UIOSizeType::PX, 1))
					.constrainWidth(UIOSizeType(UIOSizeType::PX, 60))
					.get());

				luaTestPtr->lua.printFunction = [outputTextPtr](std::string text)
				{
					std::vector<std::string> lines;
					split(0, text, lines, '\n', true);
					for (auto& line : lines) {
						outputTextPtr->text.addLine(line);
					}

					outputTextPtr->text.moveCursor(glm::ivec2(0, lines.size()));

					return BIND_RESULT::CONTINUE;
				};

				// File Name Field
				luaControlPtr->addElement(std::move(fileText));
			}
		}
	}

	// wasd movement in world
	{
		UniqueReference<UIOBase, UIOInvisible> movement = refMan->makeUniqueRef<UIOInvisible>();

		movement.get()->addGlobalBind({ ControlState::CONTROLS::TEST_LEFT, ControlState::CONTROLSTATE_PRESSED | ControlState::CONTROLSTATE_DOWN }, [&](UIOCallBackParams& state, UIOBase* self_) -> CallBackBindResult
		{
			state.player.pos.x -= 1.0f;
			return BIND_RESULT::CONTINUE;
		});

		movement.get()->addGlobalBind({ ControlState::CONTROLS::TEST_RIGHT, ControlState::CONTROLSTATE_PRESSED | ControlState::CONTROLSTATE_DOWN }, [&](UIOCallBackParams& state, UIOBase* self_) -> CallBackBindResult
		{
			state.player.pos.x += 1.0f;
			return BIND_RESULT::CONTINUE;
		});

		movement.get()->addGlobalBind({ ControlState::CONTROLS::TEST_DOWN, ControlState::CONTROLSTATE_PRESSED | ControlState::CONTROLSTATE_DOWN }, [&](UIOCallBackParams& state, UIOBase* self_) -> CallBackBindResult
		{
			state.player.pos.y -= 1.0f;
			return BIND_RESULT::CONTINUE;
		});

		movement.get()->addGlobalBind({ ControlState::CONTROLS::TEST_UP, ControlState::CONTROLSTATE_PRESSED | ControlState::CONTROLSTATE_DOWN }, [&](UIOCallBackParams& state, UIOBase* self_) -> CallBackBindResult
		{
			state.player.pos.y += 1.0f;
			return BIND_RESULT::CONTINUE;
		});

		movement.get()->addGlobalBind({ ControlState::CONTROLS::ZOOM_IN, ControlState::CONTROLSTATE_PRESSED | ControlState::CONTROLSTATE_DOWN }, [&](UIOCallBackParams& state, UIOBase* self_) -> CallBackBindResult
		{
			using viewport = Option<OPTION::CL_VIEWPORTSCALE, float>;
			viewport::setVal(viewport::getVal() * 0.95f);
			return BIND_RESULT::CONTINUE;
		});

		movement.get()->addGlobalBind({ ControlState::CONTROLS::ZOOM_OUT, ControlState::CONTROLSTATE_PRESSED | ControlState::CONTROLSTATE_DOWN }, [&](UIOCallBackParams& state, UIOBase* self_) -> CallBackBindResult
		{
			using viewport = Option<OPTION::CL_VIEWPORTSCALE, float>;
			viewport::setVal(viewport::getVal() / 0.95f);
			return BIND_RESULT::CONTINUE;
		});

		this->UIs.push_back(std::move(movement));
	}
}
