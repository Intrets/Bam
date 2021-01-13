#include "common.h"

#include "UIOConstructDebugInfo.h"

#include "UIOConstructer2.h"
#include "Enums.h"
#include "Colors.h"
#include "UIOEmpty.h"
#include "UIOSizeType.h"
#include "Option.h"
#include "UIOButton.h"
#include "PlayerState.h"
#include "UIOWindow.h"
#include "UIOConstructItemSpawner.h"
#include "UIOList.h"
#include "UIOTextDisplay.h"
#include "Timer.h"
#include "UIOBinds.h"
#include "UIOConstructActivityBuilder.h"
#include "UIOAnchoredProxy.h"
#include "BufferWrappers.h"
#include "Locator.h"
#include "Log.h"
#include "PathManager.h"
#include "MetaOperation.h"

WeakReference<UIOBase, UIOList> UIO2::constructDebugInfo() {
	auto mainList = UIO2::startList(UIO::DIR::DOWN);

	// ---------
	// Tick info
	// ---------

	UIO2::constrainHeight({ UIO::SIZETYPE::FH, 16.3f });
	UIO2::background(COLORS::UI::BACKGROUND);
	auto tickInfo = UIO2::textDisplayMulti("TODO: add bind");

	UIO2::constrainHeight({ UIO::SIZETYPE::STATIC_PX, 4 });
	UIO2::makeEnd<UIOEmpty>();

	// ---------------------
	// Save and Load buttons
	// ---------------------

	UIO2::constrainHeight({ UIO::SIZETYPE::FH, 1.2f });
	UIO2::background(COLORS::UI::BACKGROUND);
	auto saveName = UIO2::textEditSingle("test.save");

	UIO2::constrainHeight({ UIO::SIZETYPE::FH, 1.2f });
	UIO2::startList(UIO::DIR::RIGHT);

	UIO2::constrainWidth({ UIO::SIZETYPE::RELATIVE_WIDTH, 0.5f });
	auto saveButton = UIO2::textButton("save");

	auto loadButton = UIO2::textButton("load");

	UIO2::endList();

	UIO2::constrainHeight({ UIO::SIZETYPE::STATIC_PX, 4 });
	UIO2::makeEnd<UIOEmpty>();

	// ---------------------------------------------------
	// Toggles for Debug Render and Seperate Render Thread
	// ---------------------------------------------------

	UIO2::constrainHeight({ UIO::SIZETYPE::FH, 1.2f });
	auto debugButton = UIO2::textButton("Debug Render");

	UIO2::constrainHeight({ UIO::SIZETYPE::FH, 1.2f });
	auto renderThread = UIO2::textButton("Toggle Seperate Render Thread");

	// -----------------------------
	// Button for opening Builder UI
	// -----------------------------

	UIO2::constrainHeight({ UIO::SIZETYPE::ABSOLUTE_HEIGHT, 0.0f });
	auto proxy = UIO2::makeEnd<UIOAnchoredProxy>();
	UIO2::constrainHeight({ UIO::SIZETYPE::FH, 1.2f });
	auto builderTest = UIO2::textButton("Builder");

	// ----------------------------------
	// Button for opening Item Spawner UI
	// ----------------------------------

	UIO2::constrainHeight({ UIO::SIZETYPE::FH, 1.2f });
	auto spawnItem = UIO2::textButton("Spawn Item");

	// ------------------------------------------------
	// Button for opening different information windows
	// ------------------------------------------------

	//UIO2::constrainHeight({ UIO::SIZETYPE::FH, 1.2f });
	//auto info = UIO2::textButton("Info");


	UIO2::constrainHeight({ UIO::SIZETYPE::FH, 1.2f });
	UIO2::menu("Info", std::nullopt, []()
	{
		UIO2::startList(UIO::DIR::DOWN);
		UIO2::constrainHeight({ UIO::SIZETYPE::FH, 1.2f });
		UIO2::menu("Info", { { UIO::SIZETYPE::FH, 20.0f } }, []()
		{
			//UIO2::constrainHeight({ UIO::SIZETYPE::FH, 20.0f });
			UIO2::startList(UIO::DIR::DOWN);

			UIO2::constrainHeight({ UIO::SIZETYPE::FH, 10.0f });
			UIO2::textDisplayMulti(bwo::Program::listAll());

			//UIO2::constrainHeight({ UIO::SIZETYPE::FH, 1.2f });
			//UIO2::textButton("test");
			UIO2::endList();
		});
		UIO2::endList();
	});


	UIO2::constrainHeight({ UIO::SIZETYPE::STATIC_PX, 4 });
	UIO2::makeEnd<UIOEmpty>();

	// ----------
	// Log output
	// ----------

	UIO2::background(COLORS::UI::BACKGROUND);
	auto logOutput = UIO2::textDisplayMulti("");
	UIOBinds::Base::activatable(logOutput.get());

	UIO2::endList();

	// ---
	// End
	// ---

	// -----
	// Binds
	// -----

	builderTest.get()->setOnPress([proxy](PlayerState& playerState, UIOBase* self_) -> CallBackBindResult
	{
		const ACTIVITY::TYPE types[] = {
			ACTIVITY::TYPE::PISTON,
			ACTIVITY::TYPE::GRABBER,
			ACTIVITY::TYPE::RAILCRANE,
			ACTIVITY::TYPE::LUA,
			ACTIVITY::TYPE::READER,
			ACTIVITY::TYPE::DETECTOR,
			ACTIVITY::TYPE::INCINERATOR,
			ACTIVITY::TYPE::FORWARDER,
		};

		UIO2::Global::push();

		UIO2::padTop({ UIO::SIZETYPE::PX, 3 });
		UIO2::padRight({ UIO::SIZETYPE::PX, 3 });
		UIO2::padLeft({ UIO::SIZETYPE::PX, 3 });
		UIO2::background(COLORS::DARKEN2(COLORS::UI::BACKGROUND));

		UIO2::startList(UIO::DIR::DOWN);
		for (auto type : types) {
			UIO2::constrainHeight({ UIO::SIZETYPE::FH, 1.2f });
			auto button = UIO2::textButton(ACTIVITY::GET_TYPE_NAME(type));
			button.get()->setOnRelease([type = type](PlayerState& playerState, UIOBase* self_) -> CallBackBindResult
			{
				UIO2::Global::push();

				UIO2::window("Builder", { {0.5f - 0.04f, -0.1f - 0.04f}, {1.0f - 0.04f, 1.0f - 0.04f} },
							 UIOWindow::TYPE::MINIMISE |
							 UIOWindow::TYPE::MOVE |
							 UIOWindow::TYPE::RESIZE |
							 UIOWindow::TYPE::CLOSE);
				UIO2::constructActivityBuilder(type);

				playerState.uiState.addNamedUIReplace("Builder", UIO2::Global::pop());

				return BIND::RESULT::CLOSE;
			});
		}
		UIO2::endList();

		proxy.get()->setProxy(UIO2::Global::pop(), playerState.uiState);

		return BIND::RESULT::CONTINUE;
	});

	saveButton.get()->setOnRelease(
		[saveName = saveName.get()](PlayerState& playerState, UIOBase* self_)->CallBackBindResult
	{
		if (saveName->text.getLines().size() == 0) {
			return BIND::RESULT::CONTINUE;
		}
		auto name = saveName->text.getLines().front();
		name.erase(name.end() - 1);

		playerState.gameState.saveFile = name;
		return BIND::RESULT::CONTINUE;
	});

	loadButton.get()->setOnRelease([saveName = saveName.get()](PlayerState& playerState, UIOBase* self_)->CallBackBindResult
	{
		if (saveName->text.getLines().size() == 0) {
			return BIND::RESULT::CONTINUE;
		}
		auto name = saveName->text.getLines().front();
		name.erase(name.end() - 1);

		//playerState.gameState.loadFile = name;
		//playerState.uiState.reset();

		std::unique_ptr<GameLoad> op = std::make_unique<GameLoad>();

		Locator<Log>::ref().putLine("loading: " + name);

		std::ifstream save;
		Locator<PathManager>::ref().openSave(save, name);

		op->saveBuffer << save.rdbuf();
		playerState.metaActions.operations.push_back(std::move(op));

		return BIND::RESULT::CONTINUE;
	});

	tickInfo.get()->addGlobalBind(
		{ CONTROL::KEY::EVERY_TICK, static_cast<int32_t>(CONTROL::STATE::PRESSED) },
		[](PlayerState& playerState, UIOBase* self_) -> CallBackBindResult
	{
		auto self = static_cast<UIOTextDisplay*>(self_);
		self->setText(Locator<Timer>::ref().print());
		return BIND::RESULT::CONTINUE;
	});

	spawnItem.get()->setOnRelease(
		[](PlayerState& playerState, UIOBase* self_) -> CallBackBindResult
	{
		playerState.uiState.addNamedUI("Item Spawner", []()
		{
			UIO2::Global::push();

			UIO2::hideable();
			UIO2::window("Item Spawner", { { 0.5f, -1.0f }, { 1.0f , -0.2f } },
						 UIOWindow::TYPE::MINIMISE |
						 UIOWindow::TYPE::MOVE |
						 UIOWindow::TYPE::CLOSE |
						 UIOWindow::TYPE::RESIZE);
			UIO2::constructItemSpawner();

			return UIO2::Global::pop();
		});

		return BIND::RESULT::CONTINUE;
	});

	debugButton.get()->setOnRelease([](PlayerState& playerState, UIOBase* self_) -> CallBackBindResult
	{
		Option<OPTION::GR_DEBUG, bool>::setVal(!Option<OPTION::GR_DEBUG, bool>::getVal());
		auto self = static_cast<UIOButton*>(self_);
		self->setColor(Option<OPTION::GR_DEBUG, bool>::getVal() ? COLORS::UI::GREEN : COLORS::UI::RED);
		return BIND::RESULT::CONTINUE;
	});
	debugButton.get()->setColor(Option<OPTION::GR_DEBUG, bool>::getVal() ? COLORS::UI::GREEN : COLORS::UI::RED);

	renderThread.get()->setOnRelease([](PlayerState& playerState, UIOBase* self_) -> CallBackBindResult
	{
		Option<OPTION::GR_RENDERTHREAD, bool>::setVal(!Option<OPTION::GR_RENDERTHREAD, bool>::getVal());
		auto self = static_cast<UIOButton*>(self_);
		self->setColor(Option<OPTION::GR_RENDERTHREAD, bool>::getVal() ? COLORS::UI::GREEN : COLORS::UI::RED);
		return BIND::RESULT::CONTINUE;
	});
	renderThread.get()->setColor(Option<OPTION::GR_RENDERTHREAD, bool>::getVal() ? COLORS::UI::GREEN : COLORS::UI::RED);

	logOutput.get()->addGlobalBind({ CONTROL::KEY::EVERY_TICK, static_cast<int32_t>(CONTROL::STATE::PRESSED) }, [](PlayerState& playerState, UIOBase* self_) -> CallBackBindResult
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

	return mainList;
}
