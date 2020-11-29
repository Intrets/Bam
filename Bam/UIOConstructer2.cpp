#include "common.h"

#include "UIOConstructer2.h"
#include "UIOTextDisplay.h"
#include "UIOSizeType.h"
#include "UIOConstrainSize.h"
#include "UIOButton.h"
#include "UIOWindow.h"
#include "UIOCallBackParams.h"
#include "UIOGrid.h"
#include "UIOHideable.h"
#include "UIOProxy.h"
#include "UIOColoredBackground.h"
#include "UIOPad.h"
#include "UIOBinds.h"
#include "UIOFreeSize.h"
#include "UIOEmpty.h"
#include "StringHelpers.h"
#include "UIODestructible.h"

std::vector<std::unique_ptr<UIO2::ConstructerState>> UIO2::Global::states;

void UIO2::ConstructerState::addSingle(UniqueReference<UIOBase, UIOBase> ref) {
	WeakReference<UIOBase, UIOBase> weakRef = ref;

	if (this->singlesLeaf.isNotNull()) {
		this->singlesLeaf.get()->addElement(std::move(ref));
	}
	else {
		this->singlesRoot = std::move(ref);
	}

	this->singlesLeaf = weakRef;
}

void UIO2::ConstructerState::addSingle(UniqueReference<UIOBase, UIOBase> ref, WeakReference<UIOBase, UIOBase> leaf) {
	if (this->singlesLeaf.isNotNull()) {
		this->singlesLeaf.get()->addElement(std::move(ref));
	}
	else {
		this->singlesRoot = std::move(ref);
	}

	this->singlesLeaf = leaf;
}

void UIO2::ConstructerState::addMulti(UniqueReference<UIOBase, UIOBase> ref) {
	WeakReference<UIOBase, UIOBase> weakRef = ref;

	if (this->singlesLeaf.isNull()) {
		this->stack.back().get()->addElement(std::move(ref));
	}
	else {
		this->singlesLeaf.get()->addElement(std::move(ref));
		this->stack.back().get()->addElement(std::move(this->singlesRoot));

		this->singlesLeaf.clear();
		assert(this->singlesRoot.isNull());
	}
	this->stack.push_back(weakRef);
}

void UIO2::ConstructerState::addEnd(UniqueReference<UIOBase, UIOBase> ref) {
	if (this->singlesLeaf.isNull()) {
		this->stack.back().get()->addElement(std::move(ref));
	}
	else {
		this->singlesLeaf.get()->addElement(std::move(ref));
		this->stack.back().get()->addElement(std::move(this->singlesRoot));
		this->singlesLeaf.clear();
	}
}

UIO2::ConstructerState* UIO2::Global::getState() {
	return UIO2::Global::states.back().get();
}

void UIO2::Global::push() {
	UIO2::Global::states.push_back(std::make_unique<ConstructerState>());
	auto& state = UIO2::Global::states.back();

	auto ref = Locator<ReferenceManager<UIOBase>>::ref().makeUniqueRef<UIOProxy>();
	state->stack.push_back(ref);
	state->root = std::move(ref);
}

UniqueReference<UIOBase, UIOBase> UIO2::Global::pop() {
	assert(UIO2::Global::states.size() > 0);
	auto& state = UIO2::Global::states.back();

	assert(state->stack.size() == 1);
	assert(state->singlesLeaf.isNull());
	assert(state->singlesRoot.isNull());

	state->singlesLeaf.clear();
	state->stack.clear();

	auto res = std::move(state->root.get()->getMain());

	UIO2::Global::states.pop_back();

	return res;
}

UIOTextDisplay* UIO2::text(std::string const& t, bool shrinkToFit) {
	auto ref = Locator<ReferenceManager<UIOBase>>::ref().makeUniqueRef<UIOTextDisplay>();
	auto ptr = ref.get();
	ptr->setText(t);
	ptr->text.hideCursor();
	ptr->setShrinkToFit(shrinkToFit);

	UIO2::Global::getState()->addEnd(std::move(ref));

	return ptr;
}

UIOConstrainSize* UIO2::constrainHeight(UIOSizeType height) {
	auto ptr = UIO2::Global::getState()->addOrModifySingle<UIOConstrainSize>();
	ptr->setHeight(height);
	return ptr;
}

UIOConstrainSize* UIO2::constrainWidth(UIOSizeType width) {
	auto ptr = UIO2::Global::getState()->addOrModifySingle<UIOConstrainSize>();
	ptr->setWidth(width);
	return ptr;
}

UIOConstrainSize* UIO2::constrainSize(UIOSizeType size) {
	auto ptr = UIO2::Global::getState()->addOrModifySingle<UIOConstrainSize>();
	ptr->setWidth(size);
	ptr->setHeight(size);
	return ptr;
}

UIOButton* UIO2::button(bool shrinkToFit) {
	auto ref = Locator<ReferenceManager<UIOBase>>::ref().makeUniqueRef<UIOButton>();
	auto ptr = ref.get();

	ptr->setShrinkToFit(shrinkToFit);

	UIO2::Global::getState()->addSingle(std::move(ref));
	return ptr;
}

UIOWindow* UIO2::window(std::string const& title, Rectangle size, int32_t types) {
	const int32_t resizeSliverSize = 7;

	auto refMan = Locator<ReferenceManager<UIOBase>>::get();

	auto mainPad = refMan->makeUniqueRef<UIOPad>();
	WeakReference<UIOBase, UIOBase> leaf = mainPad;

	auto mainPadPtr = mainPad.get();
	mainPadPtr->top = { UIO::SIZETYPE::FH, 1.2f };
	if (types & UIOWindow::TYPE::RESIZE) {
		types |= UIOWindow::TYPE::RESIZEHORIZONTAL | UIOWindow::TYPE::RESIZEVERTICAL;
	}
	if (types & UIOWindow::TYPE::RESIZEHORIZONTAL) {
		mainPadPtr->right = { UIO::SIZETYPE::PX, resizeSliverSize };
	}
	if (types & UIOWindow::TYPE::RESIZEVERTICAL) {
		mainPadPtr->bottom = { UIO::SIZETYPE::PX, resizeSliverSize };
	}

	// ------------------
	// Main Window object
	// ------------------

	UIO2::Global::push();

	UIO2::free();
	auto windowPtr = UIO2::makeEnd<UIOWindow>();
	windowPtr->addElement(std::move(mainPad));
	UIOBinds::Base::focusable(windowPtr);
	UIOBinds::Base::blockWorldBinds(windowPtr);
	windowPtr->screenRectangle.set(size);

	UniqueReference<UIOBase, UIOBase> windowRef = UIO2::Global::pop();

	// -------
	// Top Bar
	// -------

	UIO2::Global::push();

	UIO2::alignTop();
	UIO2::constrainHeight({ UIO::SIZETYPE::FH, 1.2f });
	auto topBar = UIO2::startList(UIO::DIR::RIGHT_REVERSE);
	UIOBinds::Base::focusable(topBar);
	UIOBinds::Base::blockWorldBinds(topBar);

	// ----------------------
	// Top Bar Elements setup
	// ----------------------

	// ---------
	// Title Bar
	// ---------

	UIO2::Global::push();

	if (types & UIOWindow::TYPE::MOVE) {
		auto button = UIO2::textButton(title);
		button->addFocussedBind(
			{ CONTROL::KEY::MOUSE_POS_CHANGED, CONTROL::STATE::PRESSED },
			[windowPtr](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
		{
			auto self = static_cast<UIOButton*>(self_);
			if (self->isDown()) {
				windowPtr->moveTopLeftTo(params.uiState.getCursorPositionScreenClamped(0.99f) - self->getMousePressOffset());
			}
			return BIND::RESULT::CONTINUE;
		});
	}
	else {
		UIO2::pad({ UIO::SIZETYPE::PX, 1 });
		UIO2::textDisplaySingle(title);
	}

	topBar->addElement(UIO2::Global::pop());

	// --------
	// Minimise
	// --------

	if (types & UIOWindow::TYPE::MINIMISE) {
		UIO2::Global::push();

		UIO2::constrainSize({ UIO::SIZETYPE::FH, 1.2f });
		auto button = UIO2::textButton(" _");
		button->setOnRelease([windowPtr](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
		{
			windowPtr->minimized = !windowPtr->minimized;
			return BIND::RESULT::CONTINUE;
		});

		topBar->addElement(UIO2::Global::pop());
	}

	// ----------
	// Close/Hide
	// ----------

	if (types & UIOWindow::TYPE::CLOSE || types & UIOWindow::TYPE::HIDE) {
		UIO2::Global::push();

		UIO2::constrainSize({ UIO::SIZETYPE::FH, 1.2f });
		auto button = UIO2::textButton(" x");

		if (types & UIOWindow::TYPE::CLOSE) {
			UIOBinds::Button::close(button);
		}
		else if (types & UIOWindow::TYPE::HIDE) {
			UIOBinds::Button::hide(button);

		}

		topBar->addElement(UIO2::Global::pop());
	}

	UIO2::endList();
	windowPtr->addElementMulti(UIO2::Global::pop());
	windowPtr->topBar = topBar;

	// --------------------
	// Resize Buttons setup
	// --------------------

	if (types & UIOWindow::TYPE::RESIZEVERTICAL) {
		UIO2::Global::push();

		UIO2::alignBottomLeft();
		UIO2::constrainHeight({ UIO::SIZETYPE::PX, resizeSliverSize });
		UIO2::padRight({ UIO::SIZETYPE::PX, types & UIOWindow::TYPE::RESIZEHORIZONTAL ? resizeSliverSize : 0 });
		UIO2::padLeft({ UIO::SIZETYPE::PX, 1 });
		UIO2::padTop({ UIO::SIZETYPE::PX, 1 });
		auto button = UIO2::button();
		UIO2::makeEnd<UIOEmpty>();

		button->addFocussedBind(
			{ CONTROL::KEY::MOUSE_POS_CHANGED, CONTROL::STATE::PRESSED },
			[windowPtr](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
		{
			auto self = static_cast<UIOButton*>(self_);
			if (self->isDown()) {
				auto bottomRight = windowPtr->screenRectangle.getBottomRight();
				bottomRight.y =
					params.uiState.getCursorPositionScreenClamped(0.99f).y
					- self->getMousePressOffset().y - self->screenRectangle.getAbsSize().y;
				if (windowPtr->screenRectangle.getTop() - bottomRight.y < 0.2f) {
					bottomRight.y = windowPtr->screenRectangle.getTop() - 0.2f;
				}
				windowPtr->screenRectangle.setBottomRight(bottomRight);
				windowPtr->updateSize(windowPtr->screenRectangle);
			}
			return BIND::RESULT::CONTINUE;
		});

		windowPtr->addElementMulti(UIO2::Global::pop());
	}

	if (types & UIOWindow::TYPE::RESIZEHORIZONTAL) {
		UIO2::Global::push();

		UIO2::alignRight();
		UIO2::constrainWidth({ UIO::SIZETYPE::PX, resizeSliverSize });
		UIO2::padLeft({ UIO::SIZETYPE::PX, 1 });
		UIO2::padTop({ UIO::SIZETYPE::FH, 1.2f });
		UIO2::padBot({ UIO::SIZETYPE::PX, types & UIOWindow::TYPE::RESIZEVERTICAL ? resizeSliverSize : 0 });
		auto button = UIO2::button();
		UIO2::makeEnd<UIOEmpty>();

		button->addFocussedBind(
			{ CONTROL::KEY::MOUSE_POS_CHANGED, CONTROL::STATE::PRESSED },
			[windowPtr](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
		{
			auto self = static_cast<UIOButton*>(self_);
			if (self->isDown()) {
				auto bottomRight = windowPtr->screenRectangle.getBottomRight();
				bottomRight.x =
					params.uiState.getCursorPositionScreenClamped(0.99f).x
					- self->getMousePressOffset().x + self->screenRectangle.getAbsSize().x;
				if (bottomRight.x - windowPtr->screenRectangle.getLeft() < 0.2f) {
					bottomRight.x = windowPtr->screenRectangle.getLeft() + 0.2f;
				}
				windowPtr->screenRectangle.setBottomRight(bottomRight);
				windowPtr->updateSize(windowPtr->screenRectangle);
			}
			return BIND::RESULT::CONTINUE;
		});

		windowPtr->addElementMulti(UIO2::Global::pop());
	}

	if (types & UIOWindow::TYPE::RESIZE) {
		UIO2::Global::push();

		UIO2::alignBottomRight();
		UIO2::constrainWidth({ UIO::SIZETYPE::PX, resizeSliverSize });
		UIO2::constrainHeight({ UIO::SIZETYPE::PX, resizeSliverSize });
		auto button = UIO2::button();
		UIO2::makeEnd<UIOEmpty>();

		button->addFocussedBind(
			{ CONTROL::KEY::MOUSE_POS_CHANGED, CONTROL::STATE::PRESSED },
			[windowPtr](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
		{
			auto self = static_cast<UIOButton*>(self_);
			if (self->isDown()) {
				auto bottomRight =
					params.uiState.getCursorPositionScreenClamped(0.99f)
					- self->getMousePressOffset() + glm::vec2(1.0f, -1.0f) * self->screenRectangle.getAbsSize();
				if (bottomRight.x - windowPtr->screenRectangle.getLeft() < 0.2f) {
					bottomRight.x = windowPtr->screenRectangle.getLeft() + 0.2f;
				}
				if (windowPtr->screenRectangle.getTop() - bottomRight.y < 0.2f) {
					bottomRight.y = windowPtr->screenRectangle.getTop() - 0.2f;
				}
				windowPtr->screenRectangle.setBottomRight(bottomRight);
				windowPtr->updateSize(windowPtr->screenRectangle);
			}
			return BIND::RESULT::CONTINUE;
		});

		windowPtr->addElementMulti(UIO2::Global::pop());
	}

	UIO2::Global::getState()->addSingle(std::move(windowRef), leaf);

	return windowPtr;
}

UIOHideable* UIO2::hideable() {
	auto ref = Locator<ReferenceManager<UIOBase>>::ref().makeUniqueRef<UIOHideable>();
	auto ptr = ref.get();

	UIO2::Global::getState()->addSingle(std::move(ref));

	return ptr;
}

UIOColoredBackground* UIO2::background(glm::vec4 color) {
	auto ptr = UIO2::Global::getState()->addSingle<UIOColoredBackground>();
	ptr->color = color;
	return ptr;
}

UIOConstrainSize* UIO2::align(UIO::ALIGNMENT alignment) {
	auto ptr = UIO2::Global::getState()->addOrModifySingle<UIOConstrainSize>();
	ptr->setAlignment(alignment);
	return ptr;
}

UIOConstrainSize* UIO2::alignCenter() {
	return UIO2::align(UIO::ALIGNMENT::CENTER);
}

UIOConstrainSize* UIO2::alignTop() {
	return UIO2::align(UIO::ALIGNMENT::TOP);
}

UIOConstrainSize* UIO2::alignBottom() {
	return UIO2::align(UIO::ALIGNMENT::BOTTOM);
}

UIOConstrainSize* UIO2::alignLeft() {
	return UIO2::align(UIO::ALIGNMENT::LEFT);
}

UIOConstrainSize* UIO2::alignRight() {
	return UIO2::align(UIO::ALIGNMENT::RIGHT);
}

UIOConstrainSize* UIO2::alignBottomLeft() {
	return UIO2::align(UIO::ALIGNMENT::BOTTOMLEFT);
}

UIOConstrainSize* UIO2::alignBottomRight() {
	return UIO2::align(UIO::ALIGNMENT::BOTTOMRIGHT);
}

UIOConstrainSize* UIO2::alignTopLeft() {
	return UIO2::align(UIO::ALIGNMENT::TOPLEFT);
}

UIOConstrainSize* UIO2::alignTopRight() {
	return UIO2::align(UIO::ALIGNMENT::TOPRIGHT);
}

UIOFreeSize* UIO2::free() {
	auto ref = Locator<ReferenceManager<UIOBase>>::ref().makeUniqueRef<UIOFreeSize>();
	auto ptr = ref.get();

	UIO2::Global::getState()->addSingle(std::move(ref));

	return ptr;
}

UIODestructible* UIO2::destructible() {
	auto ref = Locator<ReferenceManager<UIOBase>>::ref().makeUniqueRef<UIODestructible>();
	auto ptr = ref.get();

	UIO2::Global::getState()->addSingle(std::move(ref));

	return ptr;
}

UIOPad* UIO2::pad(UIOSizeType padding) {
	auto ptr = UIO2::Global::getState()->addOrModifySingle<UIOPad>();
	ptr->left = padding;
	ptr->right = padding;
	ptr->top = padding;
	ptr->bottom = padding;
	return ptr;
}

UIOPad* UIO2::padTop(UIOSizeType padding) {
	auto ptr = UIO2::Global::getState()->addOrModifySingle<UIOPad>();
	ptr->top = padding;
	return ptr;
}

UIOPad* UIO2::padBot(UIOSizeType padding) {
	auto ptr = UIO2::Global::getState()->addOrModifySingle<UIOPad>();
	ptr->bottom = padding;
	return ptr;
}

UIOPad* UIO2::padLeft(UIOSizeType padding) {
	auto ptr = UIO2::Global::getState()->addOrModifySingle<UIOPad>();
	ptr->left = padding;
	return ptr;
}

UIOPad* UIO2::padRight(UIOSizeType padding) {
	auto ptr = UIO2::Global::getState()->addOrModifySingle<UIOPad>();
	ptr->right = padding;
	return ptr;
}

UIOList* UIO2::startList(UIO::DIR dir) {
	auto ref = Locator<ReferenceManager<UIOBase>>::ref().makeUniqueRef<UIOList>(dir);
	auto ptr = ref.get();

	UIO2::Global::getState()->addMulti(std::move(ref));

	return ptr;
}

UIOGrid* UIO2::startGrid(int32_t x, int32_t y) {
	auto ref = Locator<ReferenceManager<UIOBase>>::ref().makeUniqueRef<UIOGrid>(glm::ivec2(x, y));
	auto ptr = ref.get();

	UIO2::Global::getState()->addMulti(std::move(ref));

	return ptr;
}

UIOButton* UIO2::textButton(std::string const& text) {
	return UIO2::textButton2(text).first;
}

std::pair<UIOButton*, UIOTextDisplay*> UIO2::textButton2(std::string const& text_) {
	UIO2::pad({ UIO::SIZETYPE::PX, 1 });
	auto button = UIO2::button();
	UIO2::alignCenter();
	auto text = UIO2::textDisplaySingle(text_);

	return { button, text };
}

UIOTextDisplay* UIO2::textEditSingle(std::string const& text) {
	auto res = Locator<ReferenceManager<UIOBase>>::ref().makeUniqueRef<UIOTextDisplay>();
	auto ptr = res.get();
	ptr->text.addLine(text);

	UIOBinds::TextEdit::clickSelect(ptr);
	UIOBinds::Base::activatable(ptr);

	UIOBinds::TextEdit::left(ptr);
	UIOBinds::TextEdit::right(ptr);

	UIOBinds::TextEdit::inputNoLineBreaks(ptr);

	UIOBinds::TextEdit::backspace(ptr);
	UIOBinds::TextEdit::del(ptr);
	UIOBinds::TextEdit::tab(ptr);

	UIO2::makeEnd(std::move(res));
	return ptr;
}

UIOTextDisplay* UIO2::textEditMulti(std::vector<std::string> const& text, bool lineWrap) {
	auto res = Locator<ReferenceManager<UIOBase>>::ref().makeUniqueRef<UIOTextDisplay>(lineWrap);
	auto ptr = res.get();

	ptr->text.empty();
	for (auto& line : text) {
		ptr->text.addLine(line);
	}

	UIOBinds::TextEdit::clickSelect(ptr);
	UIOBinds::Base::activatable(ptr);

	UIOBinds::TextEdit::up(ptr);
	UIOBinds::TextEdit::down(ptr);
	UIOBinds::TextEdit::left(ptr);
	UIOBinds::TextEdit::right(ptr);

	UIOBinds::TextEdit::input(ptr);

	UIOBinds::TextEdit::backspace(ptr);
	UIOBinds::TextEdit::del(ptr);
	UIOBinds::TextEdit::tab(ptr);

	UIOBinds::TextEdit::viewDown(ptr);
	UIOBinds::TextEdit::viewUp(ptr);

	UIO2::makeEnd(std::move(res));

	return ptr;
}

UIOTextDisplay* UIO2::textEditMulti(std::string const& text, bool lineWrap) {
	std::vector<std::string> lines;
	split(0, text, lines, '\n', true);
	return UIO2::textEditMulti(lines, lineWrap);
}

UIOTextDisplay* UIO2::textDisplaySingle(std::string const& text, bool shrinkToFit) {
	auto res = Locator<ReferenceManager<UIOBase>>::ref().makeUniqueRef<UIOTextDisplay>(true);
	auto ptr = res.get();
	ptr->text.setString(text);
	ptr->text.hideCursor();
	ptr->setShrinkToFit(shrinkToFit);

	UIO2::makeEnd(std::move(res));

	return ptr;
}

UIOTextDisplay* UIO2::textDisplayMulti(std::vector<std::string> const& text, bool lineWrap) {
	auto res = Locator<ReferenceManager<UIOBase>>::ref().makeUniqueRef<UIOTextDisplay>(lineWrap);
	auto ptr = res.get();
	ptr->text.setLines(text);

	UIOBinds::TextEdit::viewDown(ptr);
	UIOBinds::TextEdit::viewUp(ptr);

	UIO2::makeEnd(std::move(res));

	return ptr;
}

UIOTextDisplay* UIO2::textDisplayMulti(std::string const& text, bool lineWrap) {
	std::vector<std::string> temp{ text };
	return UIO2::textDisplayMulti(temp, lineWrap);
}

void UIO2::endList() {
	UIO2::Global::getState()->pop<UIOList>();
}

void UIO2::endGrid() {
	UIO2::Global::getState()->pop<UIOGrid>();
}

