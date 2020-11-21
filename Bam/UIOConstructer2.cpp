#include "common.h"

#include "UIOConstructer2.h"
#include "UIOTextDisplay.h"
#include "UIOSizeType.h"
#include "UIOConstrainSize.h"
#include "UIOButton.h"
#include "UIOConstructer.h"
#include "UIOCallBackParams.h"
#include "UIOGrid.h"
#include "UIOHideable.h"
#include "UIOProxy.h"
#include "UIOColoredBackground.h"
#include "UIOPad.h"
#include "UIOBinds.h"

//UniqueReference<UIOBase, UIOProxy> UIO2::Global::root;
//UniqueReference<UIOBase, UIOBase> UIO2::Global::singlesRoot;
//std::vector<WeakReference<UIOBase, UIOBase>> UIO2::Global::stack;
//WeakReference<UIOBase, UIOBase> UIO2::Global::singlesLeaf;

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

	UIOWindow* windowPtr;
	auto window = UIOConstructer<UIOWindow>::makeConstructer(std::move(mainPad))
		.setPtr(windowPtr)
		.addBind(UIOBinds::Base::focusable)
		.addBind(UIOBinds::Base::blockWorldBinds)
		.free();
	windowPtr->screenRectangle.set(size);

	UIOList* topList;
	auto topBar = UIOConstructer<UIOList>::makeConstructer(UIO::DIR::LEFT)
		.setPtr(topList)
		.addBind(UIOBinds::Base::focusable)
		.addBind(UIOBinds::Base::blockWorldBinds)
		.constrainHeight({ UIO::SIZETYPE::FH, 1.2f })
		.align(UIO::ALIGNMENT::TOP)
		.get();

	windowPtr->topBar = topBar.get();
	windowPtr->addElementMulti(std::move(topBar));

	if (types & UIOWindow::TYPE::CLOSE) {
		auto close = TextConstructer::constructSingleLineDisplayText(" x")
			.align(UIO::ALIGNMENT::CENTER)
			.button()
			.onRelease([](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
		{
			return BIND::RESULT::CLOSE;
		})
			.pad({ UIO::SIZETYPE::STATIC_PX, 1 })
			.constrainHeight({ UIO::SIZETYPE::FH, 1.2f })
			.constrainWidth({ UIO::SIZETYPE::FH, 1.2f })
			.get();

		topList->addElement(std::move(close));
	}
	else if (types & UIOWindow::TYPE::HIDE) {
		auto hide = TextConstructer::constructSingleLineDisplayText(" x")
			.align(UIO::ALIGNMENT::CENTER)
			.button()
			.onRelease([](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
		{
			return BIND::RESULT::HIDE;
		})
			.pad({ UIO::SIZETYPE::STATIC_PX, 1 })
			.constrainHeight({ UIO::SIZETYPE::FH, 1.2f })
			.constrainWidth({ UIO::SIZETYPE::FH, 1.2f })
			.get();

		topList->addElement(std::move(hide));

	}

	if (types & UIOWindow::TYPE::MINIMISE) {
		auto close = TextConstructer::constructSingleLineDisplayText(" _")
			.align(UIO::ALIGNMENT::CENTER)
			.button()
			.onRelease([windowPtr](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
		{
			windowPtr->minimized = !windowPtr->minimized;
			return BIND::RESULT::CONTINUE;
		})
			.pad({ UIO::SIZETYPE::STATIC_PX, 1 })
			.constrainHeight({ UIO::SIZETYPE::FH, 1.2f })
			.constrainWidth({ UIO::SIZETYPE::FH, 1.2f })
			.get();

		topList->addElement(std::move(close));
	}

	UIOButton* titleBarPtr;
	if (types & UIOWindow::TYPE::MOVE) {
		auto titleBar = TextConstructer::constructSingleLineDisplayText(title)
			.button()
			.addFocussedBind(
				{ CONTROL::KEY::MOUSE_POS_CHANGED, CONTROL::STATE::PRESSED },
				[windowPtr](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
		{
			auto self = static_cast<UIOButton*>(self_);
			if (self->isDown()) {
				windowPtr->moveTopLeftTo(params.uiState.getCursorPositionScreenClamped(0.99f) - self->getMousePressOffset());
			}
			return BIND::RESULT::CONTINUE;
		})
			.setPtr(titleBarPtr)
			.constrainHeight({ UIO::SIZETYPE::FH, 1.2f })
			.get();
		topList->addElement(std::move(titleBar));
	}
	else {
		auto titleBar = TextConstructer::constructSingleLineDisplayText(title, false)
			.background(COLORS::UI::FOREGROUND)
			.constrainHeight({ UIO::SIZETYPE::FH, 1.2f })
			.get();
		topList->addElement(std::move(titleBar));
	}

	if (types & UIOWindow::TYPE::RESIZEVERTICAL) {
		auto bottomBar = UIOConstructer<UIOButton>::makeConstructer()
			.addFocussedBind(
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
		})
			.padLeft({ UIO::SIZETYPE::STATIC_PX, 1 })
			.padTop({ UIO::SIZETYPE::STATIC_PX, 1 })
			.padRight({ UIO::SIZETYPE::PX, types & UIOWindow::TYPE::RESIZEHORIZONTAL ? resizeSliverSize : 0 })
			.constrainHeight({ UIO::SIZETYPE::PX, resizeSliverSize })
			.align(UIO::ALIGNMENT::BOTTOMLEFT)
			.get();

		windowPtr->addElementMulti(std::move(bottomBar));
	}

	if (types & UIOWindow::TYPE::RESIZEHORIZONTAL) {
		auto rightBar = UIOConstructer<UIOButton>::makeConstructer()
			.addFocussedBind(
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
		})
			.padLeft({ UIO::SIZETYPE::STATIC_PX, 1 })
			.padTop({ UIO::SIZETYPE::FH, 1.2f })
			.padBottom({ UIO::SIZETYPE::PX, types & UIOWindow::TYPE::RESIZEVERTICAL ? resizeSliverSize : 0 })
			.constrainWidth({ UIO::SIZETYPE::PX, resizeSliverSize })
			.align(UIO::ALIGNMENT::RIGHT)
			.get();

		windowPtr->addElementMulti(std::move(rightBar));
	}

	if (types & UIOWindow::TYPE::RESIZE) {
		auto cornerBar = UIOConstructer<UIOButton>::makeConstructer()
			.addFocussedBind(
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
		})
			.constrainWidth({ UIO::SIZETYPE::PX, resizeSliverSize })
			.constrainHeight({ UIO::SIZETYPE::PX, resizeSliverSize })
			.align(UIO::ALIGNMENT::BOTTOMRIGHT)
			.get();

		windowPtr->addElementMulti(std::move(cornerBar));
	}

	UIO2::Global::getState()->addSingle(std::move(window.get()), leaf);

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
	UIO2::pad({ UIO::SIZETYPE::PX, 1 });
	auto ptr = UIO2::button();
	UIO2::alignCenter();
	UIO2::makeEnd(TextConstructer::constructSingleLineDisplayText(text).get());

	return ptr;
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

UIOTextDisplay* UIO2::textEditMulti(std::vector<std::string> const& text) {
	auto res = Locator<ReferenceManager<UIOBase>>::ref().makeUniqueRef<UIOTextDisplay>();
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

UIOTextDisplay* UIO2::textDisplaySingle(std::string const& text, bool shrinkToFit) {
	auto res = Locator<ReferenceManager<UIOBase>>::ref().makeUniqueRef<UIOTextDisplay>(true);
	auto ptr = res.get();
	ptr->text.setString(text);
	ptr->text.hideCursor();
	ptr->setShrinkToFit(shrinkToFit);

	UIO2::makeEnd(std::move(res));

	return ptr;
}

UIOTextDisplay* UIO2::textDisplayMulti(std::vector<std::string> const& text) {
	auto res = Locator<ReferenceManager<UIOBase>>::ref().makeUniqueRef<UIOTextDisplay>(true);
	auto ptr = res.get();
	ptr->text.setLines(text);

	UIOBinds::TextEdit::viewDown(ptr);
	UIOBinds::TextEdit::viewUp(ptr);

	UIO2::makeEnd(std::move(res));

	return ptr;
}

UIOTextDisplay* UIO2::textDisplayMulti(std::string const& text) {
	std::vector<std::string> temp{ text };
	return UIO2::textDisplayMulti(temp);
}

void UIO2::endList() {
	UIO2::Global::getState()->pop<UIOList>();
}

void UIO2::endGrid() {
	UIO2::Global::getState()->pop<UIOGrid>();
}
