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

	auto uioLua = UIOConstructer<UIOLua>::makeConstructer(ref).get();
	auto uioLuaPtr = uioLua.get();

	listPtr->addElement(std::move(uioLua));

	UIOTextDisplay* luaTextPtr;
	{
		auto luaEditor = TextConstructer::constructTextEdit("-- lua")
			.setPtr(luaTextPtr)
			.background(COLORS::UI::BACKGROUND)
			.constrainHeight(UIOSizeType(UIOSizeType::RELATIVE_HEIGHT, 0.4f))
			.get();

		listPtr->addElement(std::move(luaEditor));
	}

	// list of vars to be watched
	UIOTextDisplay* watchTextPtr;
	// text window to display watched vars
	UIOTextDisplay* displayWatchTextPtr;
	UIOTextDisplay* outputTextPtr;
	// name of .lua file

	{
		UIOGrid* watchGridPtr;
		auto watchGrid = UIOConstructer<UIOGrid>::makeConstructer(glm::ivec2(2, 1))
			.setPtr(watchGridPtr)
			.constrainHeight(UIOSizeType(UIOSizeType::RELATIVE_HEIGHT, 0.8f))
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

	// Push Pull Run Interrupt
	{
		UIOGrid* row1Ptr;
		listPtr->addElement(
			UIOConstructer<UIOGrid>::makeConstructer(glm::ivec2(4, 1))
			.setPtr(row1Ptr)
			.padTop(UIOSizeType(UIOSizeType::PX, 1))
			.constrainHeight(UIOSizeType(UIOSizeType::FH, 1.2f))
			.get()
		);

		row1Ptr->addElement(
			TextConstructer::constructSingleLineDisplayText("Push")
			.align(UIOConstrainSize::ALIGNMENT::CENTER)
			.button()
			.onRelease([luaTextPtr, uioLuaPtr](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
		{
			if (uioLuaPtr->getWatched().isValid()) {
				uioLuaPtr->getWatched().get()->lua.setScript(join(luaTextPtr->text.getLines()), params.gameState);
			}
			return BIND_RESULT::CONTINUE;
		})
			.pad(UIOSizeType(UIOSizeType::STATIC_PX, 1))
			.get()
			);

		row1Ptr->addElement(
			TextConstructer::constructSingleLineDisplayText("Pull")
			.align(UIOConstrainSize::ALIGNMENT::CENTER)
			.button()
			.onRelease([luaTextPtr, uioLuaPtr](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
		{
			if (uioLuaPtr->getWatched().isValid()) {
				luaTextPtr->text.getLinesMutable().clear();
				split(0, uioLuaPtr->getWatched().get()->lua.getScript(), luaTextPtr->text.getLinesMutable(), '\n', true, true);
				luaTextPtr->text.invalidateCache();
			}
			return BIND_RESULT::CONTINUE;
		})
			.pad(UIOSizeType(UIOSizeType::STATIC_PX, 1))
			.get()
			);

		row1Ptr->addElement(
			TextConstructer::constructSingleLineDisplayText("Run")
			.align(UIOConstrainSize::ALIGNMENT::CENTER)
			.button()
			.onRelease([luaTextPtr, uioLuaPtr](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
		{
			if (uioLuaPtr->getWatched().isValid()) {
				uioLuaPtr->getWatched().get()->start(params.gameState);
			}

			return BIND_RESULT::CONTINUE;
		})
			.pad(UIOSizeType(UIOSizeType::STATIC_PX, 1))
			.get()
			);

		row1Ptr->addElement(
			TextConstructer::constructSingleLineDisplayText("Interrupt")
			.align(UIOConstrainSize::ALIGNMENT::CENTER)
			.button()
			.onRelease([luaTextPtr, uioLuaPtr](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
		{
			if (uioLuaPtr->getWatched().isValid()) {
				uioLuaPtr->getWatched().get()->stop();
			}
			return BIND_RESULT::CONTINUE;
		})
			.pad(UIOSizeType(UIOSizeType::STATIC_PX, 1))
			.get()
			);
	}

	// File Name - Save - Load
	{
		UIOGrid* luaControlPtr;
		listPtr->addElement(
			UIOConstructer<UIOGrid>::makeConstructer(glm::ivec2(3, 1))
			.setPtr(luaControlPtr)
			.padTop(UIOSizeType(UIOSizeType::PX, 1))
			.constrainHeight(UIOSizeType(UIOSizeType::FH, 1.2f))
			.get()
		);

		UIOTextDisplay* fileTextPtr;
		luaControlPtr->addElement(
			TextConstructer::constructSingleLineTextEdit("test.lua")
			.setPtr(fileTextPtr)
			.background(COLORS::UI::BACKGROUND)
			.get()
		);

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

		uioLuaPtr->getWatched().get()->lua.setPrintFunction(
			[outputTextPtr](std::string text)
		{
			std::vector<std::string> lines;
			split(0, text, lines, '\n', true);
			for (auto& line : lines) {
				outputTextPtr->text.addLine(line);
			}

			outputTextPtr->text.moveCursor(glm::ivec2(0, lines.size()));

			return BIND_RESULT::CONTINUE;
		});
	}

	return window;
}
