#include "common.h"

#include "UIOConstructLuaInterface.h"
#include "UIOConstructer.h"
#include "UIOLua.h"
#include "UIOGrid.h"
#include "UIOCallBackParams.h"
#include <fstream>
#include "StringHelpers.h"
#include "UIOConstructer2.h"

UIOList* UIO2::constructLuaInterface(WeakReference<Activity, LuaActivity> ref) {
	auto list = UIO2::listStart(UIO::DIR::DOWN_REVERSE);

	auto uioLua = UIO2::makeEnd<UIOLua>(ref);

	UIO2::background(COLORS::UI::BACKGROUND);
	auto luaText = UIO2::makeEnd(TextConstructer::constructTextEdit(ref.get()->getScript()).get());

	// ----------------------------------------
	// Watched Variables and Output from script
	// ----------------------------------------

	UIO2::constrainHeight({ UIO::SIZETYPE::RELATIVE_HEIGHT, 0.2f });
	UIO2::gridStart(3, 1);

	std::vector<std::string> w = { "" };
	if (uioLua->getWatched().isValid()) {
		w = uioLua->getWatched().get()->getWatchedVars();
		if (w.empty()) {
			w = { "" };
		}
	}
	UIO2::pad({ UIO::SIZETYPE::STATIC_PX, 1 });
	UIO2::background(COLORS::UI::BACKGROUND);
	auto watchText = UIO2::makeEnd(TextConstructer::constructTextEdit(w).get());

	UIO2::pad({ UIO::SIZETYPE::STATIC_PX, 1 });
	UIO2::background(COLORS::UI::BACKGROUND);
	auto displayWatchText = UIO2::makeEnd(TextConstructer::constructDisplayText("watch").get());

	UIO2::pad({ UIO::SIZETYPE::STATIC_PX, 1 });
	UIO2::background(COLORS::UI::BACKGROUND);
	auto outputText = UIO2::makeEnd(TextConstructer::constructDisplayText("output").get());

	UIO2::end();

	// ---------------------------------------------
	// Control Buttons: Save/Load Pull/Push filename
	// ---------------------------------------------

	UIO2::constrainHeight({ UIO::SIZETYPE::FH, 1.2f });
	UIO2::padTop({ UIO::SIZETYPE::PX, 1 });
	UIO2::gridStart(4, 1);

	auto pushButton = UIO2::textButton("Push");

	auto pullButton = UIO2::textButton("Pull");

	auto runButton = UIO2::textButton("Run");

	auto interruptButton = UIO2::textButton("Interrupt");

	UIO2::end();

	UIO2::constrainHeight({ UIO::SIZETYPE::FH, 1.2f });
	UIO2::padTop({ UIO::SIZETYPE::PX, 1 });
	UIO2::gridStart(3, 1);

	UIO2::background(COLORS::UI::BACKGROUND);
	auto saveFileName = UIO2::makeEnd(TextConstructer::constructSingleLineTextEdit("test.lua").get());

	auto loadButton = UIO2::textButton("Load");

	auto saveButton = UIO2::textButton("Save");

	UIO2::end();

	UIO2::end();

	// -----
	// Binds
	// -----

	watchText->addActiveBind({ CONTROL::KEY::ANYTHING_TEXT }, [uioLua](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
	{
		auto self = static_cast<UIOTextDisplay*>(self_);

		if (uioLua->getWatched().isValid()) {
			std::vector<std::string> result;
			for (auto const& line : self->text.getLines()) {
				if (line.size() > 1) {
					result.push_back(line.substr(0, line.size() - 1));
				}
			}
			uioLua->getWatched().get()->setWatchedVars(result);
		}
		return BIND::CONTINUE;
	});

	displayWatchText->addGlobalBind({ CONTROL::KEY::EVERY_TICK }, [uioLua, watchText](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
	{
		auto self = static_cast<UIOTextDisplay*>(self_);

		if (uioLua->getWatched().isValid()) {
			auto lua = uioLua->getWatched().get();
			self->text.empty();
			for (auto& line : lua->getWatchedVars()) {
				sol::object object = lua->getLuaObject(line);
				auto type = object.get_type();
				std::string out = "invalid";

				switch (type) {
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
					case sol::type::boolean:
						out = object.as<bool>() ? "true" : "false";
						break;
					case sol::type::table:
						out = "table";
						break;
					case sol::type::none:
					case sol::type::lua_nil:
					case sol::type::thread:
					case sol::type::function:
					case sol::type::userdata:
					case sol::type::lightuserdata:
					case sol::type::poly:
					default:
						break;
				}

				self->text.addLine(line + ": " + out);
			}
		}
		return BIND::CONTINUE;
	});

	pushButton->setOnRelease([luaText, uioLua](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
	{
		if (uioLua->getWatched().isValid()) {
			uioLua->getWatched().get()->setScript(join(luaText->text.getLines()), params.gameState);
		}
		return BIND::RESULT::CONTINUE;
	});

	pullButton->setOnRelease([luaText, uioLua](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
	{
		if (uioLua->getWatched().isValid()) {
			luaText->text.getLinesMutable().clear();
			split(0, uioLua->getWatched().get()->getScript(), luaText->text.getLinesMutable(), '\n', true, true);
			luaText->text.invalidateCache();
		}
		return BIND::RESULT::CONTINUE;
	});

	runButton->setOnRelease([luaText, uioLua](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
	{
		if (uioLua->getWatched().isValid()) {
			uioLua->getWatched().get()->start();
		}

		return BIND::RESULT::CONTINUE;
	});

	interruptButton->setOnRelease([luaText, uioLua](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
	{
		if (uioLua->getWatched().isValid()) {
			uioLua->getWatched().get()->stop();
		}
		return BIND::RESULT::CONTINUE;
	});


	loadButton->setOnRelease([saveFileName, luaText](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
	{
		std::string name = saveFileName->text.getLines().front();
		name.resize(name.size() - 1);

		std::ifstream file;
		Locator<PathManager>::ref().openLUA(file, name);

		luaText->text.empty();
		std::string line;
		while (std::getline(file, line)) {
			luaText->text.addLine(line);
		}

		return BIND::RESULT::CONTINUE;
	});

	saveButton->setOnRelease([saveFileName, luaText](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
	{
		std::string name = saveFileName->text.getLines().front();
		name.resize(name.size() - 1);

		std::ofstream file;

		Locator<PathManager>::ref().openLUA(file, name);

		for (auto const& line : luaText->text.getLines()) {
			file << line;
		}

		file.close();

		return BIND::RESULT::CONTINUE;
	});

	uioLua->getWatched().get()->setPrintFunction([display = ManagedReference<UIOBase, UIOTextDisplay>(outputText->getSelfHandle())](std::string text)
	{
		if (display.isValid()) {
			std::vector<std::string> lines;
			split(0, text, lines, '\n', true);
			for (auto& line : lines) {
				display.get()->text.addLine(line);
			}

			display.get()->text.moveCursor(glm::ivec2(0, lines.size()));
		}

		return BIND::RESULT::CONTINUE;
	});

	return list;
}
