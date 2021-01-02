#include "common.h"

#include "UIOConstructer2.h"
#include "UIOTextDisplay.h"
#include "UIOSizeType.h"
#include "UIOConstrainSize.h"
#include "UIOButton.h"
#include "UIOWindow.h"
#include "PlayerState.h"
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
#include "UIOAnchoredProxy.h"

std::vector<std::unique_ptr<UIO2::ConstructerState>> UIO2::Global::states;
ReferenceManager<UIOBase> UIO2::Global::manager;

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

ReferenceManager<UIOBase>& UIO2::Global::getManager() {
	return UIO2::Global::manager;
}

UIO2::ConstructerState* UIO2::Global::getState() {
	return UIO2::Global::states.back().get();
}

void UIO2::Global::push() {
	UIO2::Global::states.push_back(std::make_unique<ConstructerState>());
	auto& state = UIO2::Global::states.back();

	auto ref = UIO2::Global::getManager().makeUniqueRef<UIOProxy>();
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

WeakReference<UIOBase, UIOTextDisplay> UIO2::text(std::string const& t, bool shrinkToFit) {
	auto ref = UIO2::Global::getManager().makeUniqueRef<UIOTextDisplay>();
	auto res = ref.as<UIOTextDisplay>();

	ref.get()->setText(t);
	ref.get()->text.hideCursor();
	ref.get()->setShrinkToFit(shrinkToFit);

	UIO2::Global::getState()->addEnd(std::move(ref));

	return res;
}

WeakReference<UIOBase, UIOConstrainSize> UIO2::constrainHeight(UIOSizeType height) {
	auto ptr = UIO2::Global::getState()->addOrModifySingle<UIOConstrainSize>();
	ptr.get()->setHeight(height);
	return ptr;
}

WeakReference<UIOBase, UIOConstrainSize> UIO2::constrainWidth(UIOSizeType width) {
	auto ptr = UIO2::Global::getState()->addOrModifySingle<UIOConstrainSize>();
	ptr.get()->setWidth(width);
	return ptr;
}

WeakReference<UIOBase, UIOConstrainSize> UIO2::constrainSize(UIOSizeType size) {
	auto ptr = UIO2::Global::getState()->addOrModifySingle<UIOConstrainSize>();
	ptr.get()->setWidth(size);
	ptr.get()->setHeight(size);
	return ptr;
}

WeakReference<UIOBase, UIOButton> UIO2::button(bool shrinkToFit) {
	auto ref = UIO2::Global::getManager().makeUniqueRef<UIOButton>();
	auto res = ref.as<UIOButton>();

	ref.get()->setShrinkToFit(shrinkToFit);

	UIO2::Global::getState()->addSingle(std::move(ref));
	return res;
}

WeakReference<UIOBase, UIOWindow> UIO2::window(std::string const& title, Rectangle size, int32_t types) {
	const int32_t resizeSliverSize = 7;

	auto mainPad = UIO2::Global::getManager().makeUniqueRef<UIOPad>();
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
	windowPtr.get()->addElement(std::move(mainPad));
	UIOBinds::Base::focusable(windowPtr.get());
	UIOBinds::Base::blockWorldBinds(windowPtr.get());
	windowPtr.get()->screenRectangle.set(size);

	UniqueReference<UIOBase, UIOBase> windowRef = UIO2::Global::pop();

	// -------
	// Top Bar
	// -------

	UIO2::Global::push();

	UIO2::alignTop();
	UIO2::constrainHeight({ UIO::SIZETYPE::FH, 1.2f });
	auto topBar = UIO2::startList(UIO::DIR::RIGHT_REVERSE);
	UIOBinds::Base::focusable(topBar.get());
	UIOBinds::Base::blockWorldBinds(topBar.get());

	// ----------------------
	// Top Bar Elements setup
	// ----------------------

	// ---------
	// Title Bar
	// ---------

	UIO2::Global::push();

	if (types & UIOWindow::TYPE::MOVE) {
		auto button = UIO2::textButton(title);
		button.get()->addFocussedBind(
			{ CONTROL::KEY::MOUSE_POS_CHANGED, CONTROL::STATE::PRESSED },
			[windowPtr = windowPtr.get()](PlayerState& playerState, UIOBase* self_)->CallBackBindResult
		{
			auto self = static_cast<UIOButton*>(self_);
			if (self->isDown()) {
				windowPtr->moveTopLeftTo(playerState.uiState.getCursorPositionScreenClamped(0.99f) - self->getMousePressOffset());
			}
			return BIND::RESULT::CONTINUE;
		});
	}
	else {
		UIO2::pad({ UIO::SIZETYPE::PX, 1 });
		UIO2::textDisplaySingle(title);
	}

	topBar.get()->addElement(UIO2::Global::pop());

	// --------
	// Minimise
	// --------

	if (types & UIOWindow::TYPE::MINIMISE) {
		UIO2::Global::push();

		UIO2::constrainSize({ UIO::SIZETYPE::FH, 1.2f });
		auto button = UIO2::textButton(" _");
		button.get()->setOnRelease([windowPtr = windowPtr.get()](PlayerState& playerState, UIOBase* self_)->CallBackBindResult
		{
			windowPtr->minimized = !windowPtr->minimized;
			return BIND::RESULT::CONTINUE;
		});

		topBar.get()->addElement(UIO2::Global::pop());
	}

	// ----------
	// Close/Hide
	// ----------

	if (types & UIOWindow::TYPE::CLOSE || types & UIOWindow::TYPE::HIDE) {
		UIO2::Global::push();

		UIO2::constrainSize({ UIO::SIZETYPE::FH, 1.2f });
		auto button = UIO2::textButton(" x");

		if (types & UIOWindow::TYPE::CLOSE) {
			UIOBinds::Button::close(button.get());
		}
		else if (types & UIOWindow::TYPE::HIDE) {
			UIOBinds::Button::hide(button.get());

		}

		topBar.get()->addElement(UIO2::Global::pop());
	}

	UIO2::endList();
	windowPtr.get()->addElementMulti(UIO2::Global::pop());
	windowPtr.get()->topBar = topBar;

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

		button.get()->addFocussedBind(
			{ CONTROL::KEY::MOUSE_POS_CHANGED, CONTROL::STATE::PRESSED },
			[windowPtr = windowPtr.get()](PlayerState& playerState, UIOBase* self_)->CallBackBindResult
		{
			auto self = static_cast<UIOButton*>(self_);
			if (self->isDown()) {
				auto bottomRight = windowPtr->screenRectangle.getBottomRight();
				bottomRight.y =
					playerState.uiState.getCursorPositionScreenClamped(0.99f).y
					- self->getMousePressOffset().y - self->screenRectangle.getAbsSize().y;
				if (windowPtr->screenRectangle.getTop() - bottomRight.y < 0.2f) {
					bottomRight.y = windowPtr->screenRectangle.getTop() - 0.2f;
				}
				windowPtr->screenRectangle.setBottomRight(bottomRight);
				windowPtr->updateSize(windowPtr->screenRectangle);
			}
			return BIND::RESULT::CONTINUE;
		});

		windowPtr.get()->addElementMulti(UIO2::Global::pop());
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

		button.get()->addFocussedBind(
			{ CONTROL::KEY::MOUSE_POS_CHANGED, CONTROL::STATE::PRESSED },
			[windowPtr = windowPtr.get()](PlayerState& playerState, UIOBase* self_)->CallBackBindResult
		{
			auto self = static_cast<UIOButton*>(self_);
			if (self->isDown()) {
				auto bottomRight = windowPtr->screenRectangle.getBottomRight();
				bottomRight.x =
					playerState.uiState.getCursorPositionScreenClamped(0.99f).x
					- self->getMousePressOffset().x + self->screenRectangle.getAbsSize().x;
				if (bottomRight.x - windowPtr->screenRectangle.getLeft() < 0.2f) {
					bottomRight.x = windowPtr->screenRectangle.getLeft() + 0.2f;
				}
				windowPtr->screenRectangle.setBottomRight(bottomRight);
				windowPtr->updateSize(windowPtr->screenRectangle);
			}
			return BIND::RESULT::CONTINUE;
		});

		windowPtr.get()->addElementMulti(UIO2::Global::pop());
	}

	if (types & UIOWindow::TYPE::RESIZE) {
		UIO2::Global::push();

		UIO2::alignBottomRight();
		UIO2::constrainWidth({ UIO::SIZETYPE::PX, resizeSliverSize });
		UIO2::constrainHeight({ UIO::SIZETYPE::PX, resizeSliverSize });
		auto button = UIO2::button();
		UIO2::makeEnd<UIOEmpty>();

		button.get()->addFocussedBind(
			{ CONTROL::KEY::MOUSE_POS_CHANGED, CONTROL::STATE::PRESSED },
			[windowPtr = windowPtr.get()](PlayerState& playerState, UIOBase* self_)->CallBackBindResult
		{
			auto self = static_cast<UIOButton*>(self_);
			if (self->isDown()) {
				auto bottomRight =
					playerState.uiState.getCursorPositionScreenClamped(0.99f)
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

		windowPtr.get()->addElementMulti(UIO2::Global::pop());
	}

	UIO2::Global::getState()->addSingle(std::move(windowRef), leaf);

	return windowPtr;
}

WeakReference<UIOBase, UIOHideable> UIO2::hideable() {
	auto ref = UIO2::Global::getManager().makeUniqueRef<UIOHideable>();
	auto res = ref.as<UIOHideable>();

	UIO2::Global::getState()->addSingle(std::move(ref));

	return res;
}

WeakReference<UIOBase, UIOColoredBackground> UIO2::background(glm::vec4 color) {
	auto ptr = UIO2::Global::getState()->addSingle<UIOColoredBackground>();
	ptr.get()->color = color;
	return ptr;
}

WeakReference<UIOBase, UIOConstrainSize> UIO2::align(UIO::ALIGNMENT alignment) {
	auto ptr = UIO2::Global::getState()->addOrModifySingle<UIOConstrainSize>();
	ptr.get()->setAlignment(alignment);
	return ptr;
}

WeakReference<UIOBase, UIOConstrainSize> UIO2::alignCenter() {
	return UIO2::align(UIO::ALIGNMENT::CENTER);
}

WeakReference<UIOBase, UIOConstrainSize> UIO2::alignTop() {
	return UIO2::align(UIO::ALIGNMENT::TOP);
}

WeakReference<UIOBase, UIOConstrainSize> UIO2::alignBottom() {
	return UIO2::align(UIO::ALIGNMENT::BOTTOM);
}

WeakReference<UIOBase, UIOConstrainSize> UIO2::alignLeft() {
	return UIO2::align(UIO::ALIGNMENT::LEFT);
}

WeakReference<UIOBase, UIOConstrainSize> UIO2::alignRight() {
	return UIO2::align(UIO::ALIGNMENT::RIGHT);
}

WeakReference<UIOBase, UIOConstrainSize> UIO2::alignBottomLeft() {
	return UIO2::align(UIO::ALIGNMENT::BOTTOMLEFT);
}

WeakReference<UIOBase, UIOConstrainSize> UIO2::alignBottomRight() {
	return UIO2::align(UIO::ALIGNMENT::BOTTOMRIGHT);
}

WeakReference<UIOBase, UIOConstrainSize> UIO2::alignTopLeft() {
	return UIO2::align(UIO::ALIGNMENT::TOPLEFT);
}

WeakReference<UIOBase, UIOConstrainSize> UIO2::alignTopRight() {
	return UIO2::align(UIO::ALIGNMENT::TOPRIGHT);
}

WeakReference<UIOBase, UIOFreeSize> UIO2::free() {
	auto ref = UIO2::Global::getManager().makeUniqueRef<UIOFreeSize>();
	auto res = ref.as<UIOFreeSize>();

	UIO2::Global::getState()->addSingle(std::move(ref));

	return res;
}

WeakReference<UIOBase, UIODestructible> UIO2::destructible() {
	auto ref = UIO2::Global::getManager().makeUniqueRef<UIODestructible>();
	auto res = ref.as<UIODestructible>();

	UIO2::Global::getState()->addSingle(std::move(ref));

	return res;
}

WeakReference<UIOBase, UIOPad> UIO2::pad(UIOSizeType padding) {
	auto ptr = UIO2::Global::getState()->addOrModifySingle<UIOPad>();
	ptr.get()->left = padding;
	ptr.get()->right = padding;
	ptr.get()->top = padding;
	ptr.get()->bottom = padding;
	return ptr;
}

WeakReference<UIOBase, UIOPad> UIO2::padTop(UIOSizeType padding) {
	auto ptr = UIO2::Global::getState()->addOrModifySingle<UIOPad>();
	ptr.get()->top = padding;
	return ptr;
}

WeakReference<UIOBase, UIOPad> UIO2::padBot(UIOSizeType padding) {
	auto ptr = UIO2::Global::getState()->addOrModifySingle<UIOPad>();
	ptr.get()->bottom = padding;
	return ptr;
}

WeakReference<UIOBase, UIOPad> UIO2::padLeft(UIOSizeType padding) {
	auto ptr = UIO2::Global::getState()->addOrModifySingle<UIOPad>();
	ptr.get()->left = padding;
	return ptr;
}

WeakReference<UIOBase, UIOPad> UIO2::padRight(UIOSizeType padding) {
	auto ptr = UIO2::Global::getState()->addOrModifySingle<UIOPad>();
	ptr.get()->right = padding;
	return ptr;
}

WeakReference<UIOBase, UIOList> UIO2::startList(UIO::DIR dir) {
	auto ref = UIO2::Global::getManager().makeUniqueRef<UIOList>(dir);
	auto res = ref.as<UIOList>();

	UIO2::Global::getState()->addMulti(std::move(ref));

	return res;
}

WeakReference<UIOBase, UIOGrid> UIO2::startGrid(int32_t x, int32_t y) {
	auto ref = UIO2::Global::getManager().makeUniqueRef<UIOGrid>(glm::ivec2(x, y));
	auto res = ref.as<UIOGrid>();

	UIO2::Global::getState()->addMulti(std::move(ref));

	return res;
}

WeakReference<UIOBase, UIOList> UIO2::menu(std::string const& text, std::optional<UIOSizeType> width, std::function<void()> f) {
	auto list = UIO2::startList(UIO::DIR::RIGHT);

	auto button = UIO2::textButton(text);
	if (width.has_value()) {
		UIO2::constrainWidth(width.value());
	}
	else {
		UIO2::constrainWidth({ UIO::SIZETYPE::FH, 10.0f });
	}

	auto proxy = UIO2::makeEnd<UIOAnchoredProxy>();

	UIO2::endList();

	button.get()->setOnPress([proxy = proxy.get(), f](PlayerState& playerState, UIOBase* self_) -> CallBackBindResult
	{
		UIO2::Global::push();

		UIO2::padTop({ UIO::SIZETYPE::PX, 3 });
		UIO2::padRight({ UIO::SIZETYPE::PX, 3 });
		UIO2::padLeft({ UIO::SIZETYPE::PX, 3 });
		UIO2::background(COLORS::DARKEN2(COLORS::UI::BACKGROUND));

		f();

		proxy->setProxy(UIO2::Global::pop(), playerState.uiState);

		return BIND::RESULT::CONTINUE;
	});

	return list;
}

WeakReference<UIOBase, UIOButton> UIO2::textButton(std::string const& text) {
	return UIO2::textButton2(text).first;
}

std::pair<WeakReference<UIOBase, UIOButton>, WeakReference<UIOBase, UIOTextDisplay>> UIO2::textButton2(std::string const& text_) {
	UIO2::pad({ UIO::SIZETYPE::PX, 1 });
	auto button = UIO2::button();
	UIO2::alignCenter();
	auto text = UIO2::textDisplaySingle(text_);

	return { button, text };
}

WeakReference<UIOBase, UIOTextDisplay> UIO2::textEditSingle(std::string const& text) {
	auto res = UIO2::Global::getManager().makeUniqueRef<UIOTextDisplay>();
	auto ref = res.as<UIOTextDisplay>();
	auto ptr = ref.get();

	ptr->text.addLine(text);

	ptr->mode = UIOTEXTDISPLAY::MODE::INSERT;

	UIOBinds::TextEdit::clickSelect(ptr);
	UIOBinds::Base::activatable(ptr);

	UIOBinds::TextEdit::left(ptr);
	UIOBinds::TextEdit::right(ptr);

	UIOBinds::TextEdit::inputNoLineBreaks(ptr);

	UIOBinds::TextEdit::backspace(ptr);
	UIOBinds::TextEdit::del(ptr);
	UIOBinds::TextEdit::tab(ptr);

	UIO2::makeEnd(std::move(res));
	return ref;
}

WeakReference<UIOBase, UIOTextDisplay> UIO2::textEditMulti(std::vector<std::string> const& text, bool lineWrap) {
	auto ref = UIO2::Global::getManager().makeUniqueRef<UIOTextDisplay>(lineWrap);
	auto res = ref.as<UIOTextDisplay>();
	auto ptr = ref.get();

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

	UIOBinds::TextEdit::normalbinds(ptr);

	UIOBinds::TextEdit::normal(ptr);
	UIOBinds::TextEdit::insert(ptr);

	UIOBinds::TextEdit::input(ptr);

	UIOBinds::TextEdit::backspace(ptr);
	UIOBinds::TextEdit::del(ptr);
	UIOBinds::TextEdit::tab(ptr);

	UIOBinds::TextEdit::viewDown(ptr);
	UIOBinds::TextEdit::viewUp(ptr);

	UIO2::makeEnd(std::move(ref));

	return res;
}

WeakReference<UIOBase, UIOTextDisplay> UIO2::textEditMulti(std::string const& text, bool lineWrap) {
	std::vector<std::string> lines;
	split(0, text, lines, '\n', true);
	return UIO2::textEditMulti(lines, lineWrap);
}

WeakReference<UIOBase, UIOTextDisplay> UIO2::textDisplaySingle(std::string const& text, bool shrinkToFit) {
	auto ref = UIO2::Global::getManager().makeUniqueRef<UIOTextDisplay>(true);
	auto res = ref.as<UIOTextDisplay>();

	auto ptr = ref.get();
	ptr->text.setString(text);
	ptr->text.hideCursor();
	ptr->setShrinkToFit(shrinkToFit);

	UIO2::makeEnd(std::move(ref));

	return res;
}

WeakReference<UIOBase, UIOTextDisplay> UIO2::textDisplayMulti(std::vector<std::string> const& text, bool lineWrap) {
	auto ref = UIO2::Global::getManager().makeUniqueRef<UIOTextDisplay>(lineWrap);
	auto res = ref.as<UIOTextDisplay>();

	auto ptr = ref.get();
	ptr->text.setLines(text);

	UIOBinds::TextEdit::viewDown(ptr);
	UIOBinds::TextEdit::viewUp(ptr);

	UIO2::makeEnd(std::move(ref));

	return res;
}

WeakReference<UIOBase, UIOTextDisplay> UIO2::textDisplayMulti(std::string const& text, bool lineWrap) {
	std::vector<std::string> temp{ text };
	return UIO2::textDisplayMulti(temp, lineWrap);
}

void UIO2::endList() {
	UIO2::Global::getState()->pop<UIOList>();
}

void UIO2::endGrid() {
	UIO2::Global::getState()->pop<UIOGrid>();
}
