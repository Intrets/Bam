#include "common.h"

#include "UIOConstructer2.h"
#include "UIOTextDisplay.h"
#include "UIOSizeType.h"
#include "UIOConstrainSize.h"
#include "UIOButton.h"
#include "UIOConstructer.h"
#include "UIOCallBackParams.h"
#include "UIOGrid.h"

UniqueReference<UIOBase, UIOBase> UIO2::Global::root;
UniqueReference<UIOBase, UIOBase> UIO2::Global::singlesRoot;
std::vector<WeakReference<UIOBase, UIOBase>> UIO2::Global::stack;
WeakReference<UIOBase, UIOBase> UIO2::Global::singlesLeaf;


UIO2::List::List(UIO::DIR dir) {
	auto ref = Locator<ReferenceManager<UIOBase>>::ref().makeUniqueRef<UIOList>(dir);
	UIO2::Global::addMulti(std::move(ref));
}

void UIO2::Global::down() {
	UIO2::Global::stack.pop_back();
}

void UIO2::Global::start(UniqueReference<UIOBase, UIOBase> ref) {
	WeakReference<UIOBase, UIOBase> weak = ref;
	UIO2::Global::root = std::move(ref);
	UIO2::Global::stack.push_back(weak);
}

void UIO2::Global::addSingle(UniqueReference<UIOBase, UIOBase> ref) {
	WeakReference<UIOBase, UIOBase> weakRef = ref;

	if (UIO2::Global::singlesLeaf.isNotNull()) {
		UIO2::Global::singlesLeaf.get()->addElement(std::move(ref));
	}
	else {
		UIO2::Global::singlesRoot = std::move(ref);
	}

	UIO2::Global::singlesLeaf = weakRef;
}

void UIO2::Global::addSingle(UniqueReference<UIOBase, UIOBase> ref, WeakReference<UIOBase, UIOBase> leaf) {
	if (UIO2::Global::singlesLeaf.isNotNull()) {
		UIO2::Global::singlesLeaf.get()->addElement(std::move(ref));
	}
	else {
		UIO2::Global::singlesRoot = std::move(ref);
	}

	UIO2::Global::singlesLeaf = leaf;
}

void UIO2::Global::addMulti(UniqueReference<UIOBase, UIOBase> ref) {
	WeakReference<UIOBase, UIOBase> weakRef = ref;

	if (UIO2::Global::singlesLeaf.isNull()) {
		UIO2::Global::stack.back().get()->addElement(std::move(ref));
	}
	else {
		UIO2::Global::singlesLeaf.get()->addElement(std::move(ref));
		UIO2::Global::stack.back().get()->addElement(std::move(UIO2::Global::singlesRoot));

		UIO2::Global::singlesLeaf.clear();
		assert(UIO2::Global::singlesRoot.isNull());
	}
	UIO2::Global::stack.push_back(weakRef);
}

UniqueReference<UIOBase, UIOBase> UIO2::Global::finish() {
	assert(stack.size() == 1);
	return std::move(UIO2::Global::root);
}

void UIO2::text(std::string const& t) {
	auto ref = Locator<ReferenceManager<UIOBase>>::ref().makeUniqueRef<UIOTextDisplay>();
	ref.get()->setText(t);
	ref.get()->text.hideCursor();
	ref.get()->setShrinkToFit(true);

	if (UIO2::Global::singlesLeaf.isNull()) {
		UIO2::Global::stack.back().get()->addElement(std::move(ref));
	}
	else {
		UIO2::Global::singlesLeaf.get()->addElement(std::move(ref));
		UIO2::Global::stack.back().get()->addElement(std::move(UIO2::Global::singlesRoot));
		UIO2::Global::singlesLeaf.clear();
	}
}

void UIO2::constrainHeight(UIOSizeType height) {
	auto ref = Locator<ReferenceManager<UIOBase>>::ref().makeUniqueRef<UIOConstrainSize>();
	ref.get()->setHeight(height);

	UIO2::Global::addSingle(std::move(ref));
}

void UIO2::button() {
	auto ref = Locator<ReferenceManager<UIOBase>>::ref().makeUniqueRef<UIOButton>();

	UIO2::Global::addSingle(std::move(ref));
}

void UIO2::window(std::string const& title, Rectangle size, int32_t types) {
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

	UIO2::Global::addSingle(std::move(window.get()), leaf);
}

UIO2::Scope::~Scope() {
	UIO2::Global::down();
}

UIO2::Grid::Grid(int32_t x, int32_t y) {
	auto ref = Locator<ReferenceManager<UIOBase>>::ref().makeUniqueRef<UIOGrid>(glm::ivec2(x, y));
	UIO2::Global::addMulti(std::move(ref));
}
