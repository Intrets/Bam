#include "common.h"

#include "UIOConstructDebugInfo.h"
#include "UIOConstructer2.h"
#include "Enums.h"
#include "Colors.h"
#include "UIOEmpty.h"
#include "UIOSizeType.h"
#include "Option.h"
#include "UIOButton.h"
#include "UIOCallBackParams.h"
#include "UIOWindow.h"
#include "UIOConstructItemSpawner.h"
#include "UIOList.h"
#include "UIOTextDisplay.h"
#include "Timer.h"
#include "UIOBinds.h"
#include "UIOConstructActivityBuilder.h"

UIOList* UIO2::constructDebugInfo() {
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

	UIO2::constrainHeight({ UIO::SIZETYPE::FH, 1.2f });
	auto builderTest = UIO2::textButton("Builder");

	// ----------------------------------
	// Button for opening Item Spawner UI
	// ----------------------------------

	UIO2::constrainHeight({ UIO::SIZETYPE::FH, 1.2f });
	auto spawnItem = UIO2::textButton("Spawn Item");

	UIO2::constrainHeight({ UIO::SIZETYPE::STATIC_PX, 4 });
	UIO2::makeEnd<UIOEmpty>();

	// ----------
	// Log output
	// ----------

	UIO2::background(COLORS::UI::BACKGROUND);
	auto logOutput = UIO2::textDisplayMulti("");
	UIOBinds::Base::activatable(logOutput);

	UIO2::endList();

	// ---
	// End
	// ---

	// -----
	// Binds
	// -----

	builderTest->setOnRelease(
		[](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
	{
		UIO2::Global::push();

		UIO2::window("Activity Builder Test", { {0.5f - 0.04f, -0.1f - 0.04f}, {1.0f - 0.04f, 1.0f - 0.04f} },
					 UIOWindow::TYPE::MINIMISE |
					 UIOWindow::TYPE::MOVE |
					 UIOWindow::TYPE::RESIZE |
					 UIOWindow::TYPE::CLOSE);
		UIO2::constructActivityBuilder(ACTIVITY::TYPE::PISTON);

		params.uiState.addUI(UIO2::Global::pop());
		return BIND::RESULT::CONTINUE;
	});

	saveButton->setOnRelease(
		[saveName](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
	{
		if (saveName->text.getLines().size() == 0) {
			return BIND::RESULT::CONTINUE;
		}
		auto name = saveName->text.getLines().front();
		name.erase(name.end() - 1);

		params.gameState.saveFile = name;
		return BIND::RESULT::CONTINUE;
	});

	loadButton->setOnRelease([saveName](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
	{
		if (saveName->text.getLines().size() == 0) {
			return BIND::RESULT::CONTINUE;
		}
		auto name = saveName->text.getLines().front();
		name.erase(name.end() - 1);

		params.gameState.loadFile = name;
		params.uiState.reset();
		return BIND::RESULT::CONTINUE;
	});

	tickInfo->addGlobalBind(
		{ CONTROL::KEY::EVERY_TICK, static_cast<int32_t>(CONTROL::STATE::PRESSED) },
		[](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
	{
		auto self = static_cast<UIOTextDisplay*>(self_);
		self->setText(Locator<Timer>::ref().print());
		return BIND::RESULT::CONTINUE;
	});

	spawnItem->setOnRelease(
		[](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
	{
		params.uiState.addNamedUI("Item Spawner", []()
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

	debugButton->setOnRelease([](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
	{
		Option<OPTION::GR_DEBUG, bool>::setVal(!Option<OPTION::GR_DEBUG, bool>::getVal());
		auto self = static_cast<UIOButton*>(self_);
		self->setColor(Option<OPTION::GR_DEBUG, bool>::getVal() ? COLORS::UI::GREEN : COLORS::UI::RED);
		return BIND::RESULT::CONTINUE;
	});
	debugButton->setColor(Option<OPTION::GR_DEBUG, bool>::getVal() ? COLORS::UI::GREEN : COLORS::UI::RED);

	renderThread->setOnRelease([](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
	{
		Option<OPTION::GR_RENDERTHREAD, bool>::setVal(!Option<OPTION::GR_RENDERTHREAD, bool>::getVal());
		auto self = static_cast<UIOButton*>(self_);
		self->setColor(Option<OPTION::GR_RENDERTHREAD, bool>::getVal() ? COLORS::UI::GREEN : COLORS::UI::RED);
		return BIND::RESULT::CONTINUE;
	});
	renderThread->setColor(Option<OPTION::GR_RENDERTHREAD, bool>::getVal() ? COLORS::UI::GREEN : COLORS::UI::RED);

	logOutput->addGlobalBind({ CONTROL::KEY::EVERY_TICK, static_cast<int32_t>(CONTROL::STATE::PRESSED) }, [](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
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
