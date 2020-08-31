#include "common.h"

#include "UIOConstructLuaInterface.h"
#include "UIOConstructer.h"
#include "UIOLua.h"
#include "UIOGrid.h"
#include "UIOCallBackParams.h"
#include <fstream>
#include "StringHelpers.h"

UIOConstructer<UIOList> CONSTRUCTER::constructLuaInterface(WeakReference<Activity, LuaActivity> ref) {
	UIOList* listPtr;
	auto window = UIOConstructer<UIOList>::makeConstructer(UIOList::DIR::DOWN);
	window.setPtr(listPtr);

	auto luaTest = UIOConstructer<UIOLua>::makeConstructer(ref).get();
	auto luaPtr = luaTest.get();

	listPtr->addElement(std::move(luaTest));

	UIOTextDisplay* luaTextPtr;
	{
		auto luaEditor = TextConstructer::constructTextEdit("test(h ,1)")
			.setPtr(luaTextPtr)
			.background(COLORS::UI::BACKGROUND)
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
				.background(COLORS::UI::BACKGROUND)
				.pad(UIOSizeType(UIOSizeType::STATIC_PX, 1))
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
				.background(COLORS::UI::BACKGROUND)
				.pad(UIOSizeType(UIOSizeType::STATIC_PX, 1))
				.get()
			);

			outputGridPtr->addElement(
				TextConstructer::constructDisplayText("output")
				.setPtr(outputTextPtr)
				.background(COLORS::UI::BACKGROUND)
				.pad(UIOSizeType(UIOSizeType::STATIC_PX, 1))
				.get()
			);
		}

		listPtr->addElement(std::move(watchGrid));
	}

	{
		UIOGrid* luaControlPtr;
		auto luaControl = UIOConstructer<UIOGrid>::makeConstructer(glm::ivec2(6, 1))
			.setPtr(luaControlPtr)
			.padTop(UIOSizeType(UIOSizeType::STATIC_PX, 1))
			.constrainHeight(UIOSizeType(UIOSizeType::FH, 1.2f))
			.get();

		auto fileText =
			TextConstructer::constructSingleLineTextEdit("test.lua")
			.setPtr(fileTextPtr)
			.background(COLORS::UI::BACKGROUND)
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
				std::string name = fileTextPtr->text.getLines().front();
				name.resize(name.size() - 1);

				std::ifstream file;
				Locator<PathManager>::ref().openLUA(file, name);

				luaTextPtr->text.empty();
				std::string line;
				while (std::getline(file, line)) {
					luaTextPtr->text.addLine(line);
				}

				return BIND_RESULT::CONTINUE;
			})
				.pad(UIOSizeType(UIOSizeType::STATIC_PX, 1))
				.get());

			// Save
			luaControlPtr->addElement(
				TextConstructer::constructSingleLineDisplayText("Save")
				.align(UIOConstrainSize::ALIGNMENT::CENTER)
				.button()
				.onRelease([fileTextPtr, luaTextPtr](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
			{
				std::string name = fileTextPtr->text.getLines().front();
				name.resize(name.size() - 1);

				std::ofstream file;

				Locator<PathManager>::ref().openLUA(file, name);

				for (auto const& line : luaTextPtr->text.getLines()) {
					file << line;
				}

				file.close();

				return BIND_RESULT::CONTINUE;
			})
				.pad(UIOSizeType(UIOSizeType::STATIC_PX, 1))
				.get());

			// Run
			luaControlPtr->addElement(
				TextConstructer::constructSingleLineDisplayText("Run")
				.align(UIOConstrainSize::ALIGNMENT::CENTER)
				.button()
				.onRelease([luaTextPtr, luaPtr](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
			{
				if (luaPtr->getWatched().isValid()) {
					luaPtr->getWatched().get()->setScript(join(luaTextPtr->text.getLines()));
					luaPtr->getWatched().get()->start(params.gameState);
				}
				return BIND_RESULT::CONTINUE;
			})
				.pad(UIOSizeType(UIOSizeType::STATIC_PX, 1))
				.get());

			// Interrupt
			luaControlPtr->addElement(
				TextConstructer::constructSingleLineDisplayText("Interrupt")
				.align(UIOConstrainSize::ALIGNMENT::CENTER)
				.button()
				.onRelease([luaTextPtr, luaPtr](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
			{
				if (luaPtr->getWatched().isValid()) {
					luaPtr->getWatched().get()->stop();
				}
				return BIND_RESULT::CONTINUE;
			})
				.pad(UIOSizeType(UIOSizeType::STATIC_PX, 1))
				.get());

			// Read Vars
			luaControlPtr->addElement(
				TextConstructer::constructSingleLineDisplayText("Vars")
				.align(UIOConstrainSize::ALIGNMENT::CENTER)
				.button()
				.addGlobalBind({ ControlState::CONTROLS::EVERY_TICK, ControlState::CONTROLSTATE_PRESSED }, [luaPtr, watchTextPtr, displayWatchTextPtr](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
			{
				if (!luaPtr->getWatched().isValid()) {
					return BIND_RESULT::CONTINUE;
				}
				displayWatchTextPtr->text.empty();
				for (auto line : watchTextPtr->text.getLines()) {
					if (line.size() < 2) {
						continue;
					}
					line.resize(line.size() - 1);

					sol::object object = luaPtr->getWatched().get()->getLuaObject(line);
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

				return BIND_RESULT::CONTINUE;
			})
				.pad(UIOSizeType(UIOSizeType::STATIC_PX, 1))
				.get());

			ref.get()->setPrintFunction([outputTextPtr](std::string text)
			{
				std::vector<std::string> lines;
				split(0, text, lines, '\n', true);
				for (auto& line : lines) {
					outputTextPtr->text.addLine(line);
				}

				outputTextPtr->text.moveCursor(glm::ivec2(0, lines.size()));

				return BIND_RESULT::CONTINUE;
			});

			// File Name Field
			luaControlPtr->addElement(std::move(fileText));
		}
	}

	return window;
}
