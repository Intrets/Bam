#include "common.h"

#include "UIOConstructLuaInterface.h"

#include "UIOLua.h"
#include "UIOGrid.h"
#include "PlayerState.h"
#include "StringHelpers.h"
#include "UIOConstructer2.h"
#include "UIOSizeType.h"
#include "Colors.h"
#include "UIOTextDisplay.h"
#include "UIOButton.h"
#include "Locator.h"
#include "PathManager.h"

#include <fstream>
#include <memory>

WeakReference<UIOBase, UIOList> UIO2::constructLuaInterface(WeakReference<Activity, LuaActivity> ref) {
	auto list = UIO2::startList(UIO::DIR::DOWN_REVERSE);

	auto uioLua = UIO2::makeEnd<UIOLua>(ref);

	UIO2::background(COLORS::UI::BACKGROUND);
	auto luaText = UIO2::textEditMulti(ref.get()->getScript());

	// ----------------------------------------
	// Watched Variables and Output from script
	// ----------------------------------------

	UIO2::constrainHeight({ UIO::SIZETYPE::RELATIVE_HEIGHT, 0.2f });
	UIO2::startGrid(3, 1);

	std::vector<std::string> w = { "" };
	if (auto watched = uioLua.get()->getWatched().getRef()) {
		w = watched.get()->getWatchedVars();
		if (w.empty()) {
			w = { "" };
		}
	}
	UIO2::pad({ UIO::SIZETYPE::STATIC_PX, 1 });
	UIO2::background(COLORS::UI::BACKGROUND);
	auto watchText = UIO2::textEditMulti(w);

	UIO2::pad({ UIO::SIZETYPE::STATIC_PX, 1 });
	UIO2::background(COLORS::UI::BACKGROUND);
	auto displayWatchText = UIO2::textDisplayMulti("watch");

	UIO2::pad({ UIO::SIZETYPE::STATIC_PX, 1 });
	UIO2::background(COLORS::UI::BACKGROUND);
	auto outputText = UIO2::textDisplayMulti("output");

	UIO2::endGrid();

	// ---------------------------------------------
	// Control Buttons: Save/Load Pull/Push filename
	// ---------------------------------------------

	UIO2::constrainHeight({ UIO::SIZETYPE::FH, 1.2f });
	UIO2::padTop({ UIO::SIZETYPE::PX, 1 });
	UIO2::startGrid(4, 1);

	auto pushButton = UIO2::textButton("Push");

	auto pullButton = UIO2::textButton("Pull");

	auto runButton = UIO2::textButton("Run");

	auto interruptButton = UIO2::textButton("Interrupt");

	UIO2::endGrid();

	UIO2::constrainHeight({ UIO::SIZETYPE::FH, 1.2f });
	UIO2::padTop({ UIO::SIZETYPE::PX, 1 });
	UIO2::startGrid(3, 1);

	UIO2::background(COLORS::UI::BACKGROUND);
	auto saveFileName = UIO2::textEditSingle("test.lua");

	auto loadButton = UIO2::textButton("Load");

	auto saveButton = UIO2::textButton("Save");

	UIO2::endGrid();

	UIO2::endList();

	// -----
	// Binds
	// -----

	watchText.get()->addActiveBind({ CONTROL::KEY::ANYTHING_TEXT }, [uioLua = uioLua.get()](PlayerState& playerState, UIOBase* self_)->CallBackBindResult
	{
		auto self = static_cast<UIOTextDisplay*>(self_);

		if (auto watched = uioLua->getWatched().getRef()) {
			std::vector<std::string> result;
			for (auto const& line : self->text.getLines()) {
				if (line.size() > 1) {
					result.push_back(line.substr(0, line.size() - 1));
				}
			}
			watched.get()->setWatchedVars(result);
		}
		return BIND::CONTINUE;
	});

	displayWatchText.get()->addGlobalBind({ CONTROL::KEY::EVERY_TICK }, [uioLua = uioLua.get()](PlayerState& playerState, UIOBase* self_)->CallBackBindResult
	{
		auto self = static_cast<UIOTextDisplay*>(self_);

		if (auto watched = uioLua->getWatched().getRef()) {
			self->text.empty();
			for (auto& line : watched.get()->getWatchedVars()) {
				sol::object object = watched.get()->getLuaObject(line);
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

	pushButton.get()->setOnRelease([luaText = luaText.get(), uioLua = uioLua.get()](PlayerState& playerState, UIOBase* self_)->CallBackBindResult
	{
		if (auto watched = uioLua->getWatched().getRef()) {
			auto action = std::make_unique<LuaActivitySetScript>(watched.getHandle(), join(luaText->text.getLines()));
			playerState.playerActions.operations.push_back(std::move(action));
			//watched.get()->setScript(join(luaText->text.getLines()), playerState.gameState);
		}
		return BIND::RESULT::CONTINUE;
	});

	pullButton.get()->setOnRelease([luaText = luaText.get(), uioLua = uioLua.get()](PlayerState& playerState, UIOBase* self_)->CallBackBindResult
	{
		if (auto watched = uioLua->getWatched().getRef()) {
			luaText->text.getLinesMutable().clear();
			split(0, watched.get()->getScript(), luaText->text.getLinesMutable(), '\n', true, true);
			luaText->text.invalidateCache();
		}
		return BIND::RESULT::CONTINUE;
	});

	runButton.get()->setOnRelease([uioLua = uioLua.get()](PlayerState& playerState, UIOBase* self_)->CallBackBindResult
	{
		if (auto watched = uioLua->getWatched().getRef()) {
			auto action = std::make_unique<LuaActivityStart>(watched.getHandle());
			playerState.playerActions.operations.push_back(std::move(action));
			//watched.get()->start();
		}

		return BIND::RESULT::CONTINUE;
	});

	interruptButton.get()->setOnRelease([uioLua = uioLua.get()](PlayerState& playerState, UIOBase* self_)->CallBackBindResult
	{
		if (auto watched = uioLua->getWatched().getRef()) {
			auto action = std::make_unique<LuaActivityStop>(watched.getHandle());
			playerState.playerActions.operations.push_back(std::move(action));
			//watched.get()->stop();
		}

		return BIND::RESULT::CONTINUE;
	});


	loadButton.get()->setOnRelease([saveFileName = saveFileName.get(), luaText = luaText.get()](PlayerState& playerState, UIOBase* self_)->CallBackBindResult
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

	saveButton.get()->setOnRelease([saveFileName = saveFileName.get(), luaText = luaText.get()](PlayerState& playerState, UIOBase* self_)->CallBackBindResult
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

	uioLua.get()->getWatched().getRef().get()->setPrintFunction([display = ManagedReference<UIOBase, UIOTextDisplay>(outputText)](std::string text)
	{
		if (auto ref = display.getRef()) {
			std::vector<std::string> lines;
			split(0, text, lines, '\n', true);
			for (auto& line : lines) {
				ref.get()->text.addLine(line);
			}

			ref.get()->text.moveCursor(glm::ivec2(0, lines.size()));
		}

		return BIND::RESULT::CONTINUE;
	});

	return list;
}
