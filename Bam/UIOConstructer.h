#pragma once

#include "ReferenceManager.h"

#include "UIOPad.h"
#include "UIOConstrainSize.h"
#include "UIOColoredBackground.h"
#include "UIOButton.h"
#include "UIOWindow.h"
#include "UIOList.h"
#include "UIOTextDisplay.h"
#include "UIOTextConstructers.h"
#include "UIOFreeSize.h"
#include "Rectangle.h"
#include "UIOBinds.h"
#include "UIOHideable.h"

#include <type_traits>

#include "UIOWindow.h"

template<class T>
class UIOConstructer
{
private:
	UniqueReference<UIOBase, T> object;

public:
	UIOConstructer(UniqueReference<UIOBase, T> obj);
	UIOConstructer() = default;
	~UIOConstructer() = default;

	template<class B>
	[[nodiscard]] UIOConstructer<T> setPtr(B*& ptr);

	[[nodiscard]] UIOConstructer<UIOPad> pad(UIOSizeType padding);
	[[nodiscard]] UIOConstructer<UIOPad> padTop(UIOSizeType padding);
	[[nodiscard]] UIOConstructer<UIOPad> padBottom(UIOSizeType padding);
	[[nodiscard]] UIOConstructer<UIOPad> padLeft(UIOSizeType padding);
	[[nodiscard]] UIOConstructer<UIOPad> padRight(UIOSizeType padding);

	[[nodiscard]] UIOConstructer<T> addBind(std::function<void(UIOBase*)> f);
	[[nodiscard]] UIOConstructer<T> addBind(void (*f) (T* ptr));
	[[nodiscard]] UIOConstructer<T> addBindCapture(std::function<void(T*)> f);

	[[nodiscard]] UIOConstructer<T> addFocussedBind(BindControl bindControl, CallBack callBack);
	[[nodiscard]] UIOConstructer<T> addActiveBind(BindControl bindControl, CallBack callBack);
	[[nodiscard]] UIOConstructer<T> addGlobalBind(BindControl bindControl, CallBack callBack);
	[[nodiscard]] UIOConstructer<T> addOnHoverBind(BindControl bindControl, CallBack callBack);
	[[nodiscard]] UIOConstructer<T> addGameWorldBind(BindControl bindControl, CallBack callBack);

	[[nodiscard]] UIOConstructer<UIOConstrainSize> constrainHeight(UIOSizeType height);
	[[nodiscard]] UIOConstructer<UIOConstrainSize> constrainWidth(UIOSizeType width);
	[[nodiscard]] UIOConstructer<UIOConstrainSize> align(UIO::ALIGNMENT align);
	[[nodiscard]] UIOConstructer<UIOConstrainSize> alignCenter();
	[[nodiscard]] UIOConstructer<UIOConstrainSize> alignTop();
	[[nodiscard]] UIOConstructer<UIOConstrainSize> alignBottom();
	[[nodiscard]] UIOConstructer<UIOConstrainSize> alignLeft();
	[[nodiscard]] UIOConstructer<UIOConstrainSize> alignRight();
	[[nodiscard]] UIOConstructer<UIOConstrainSize> alignBottomLeft();
	[[nodiscard]] UIOConstructer<UIOConstrainSize> alignBottomRight();
	[[nodiscard]] UIOConstructer<UIOConstrainSize> alignTopLeft();
	[[nodiscard]] UIOConstructer<UIOConstrainSize> alignTopRight();

	[[nodiscard]] UIOConstructer<UIOColoredBackground> background(glm::vec4 color);
	[[nodiscard]] UIOConstructer<UIOButton> button(bool shrink = false);
	[[nodiscard]] UIOConstructer<UIOFreeSize> window(std::string title, Rectangle size, int32_t types);
	[[nodiscard]] UIOConstructer<UIOFreeSize> free();
	[[nodiscard]] UIOConstructer<UIOHideable> hideable(bool focusOnShow = true);

	[[nodiscard]] UIOConstructer<UIOButton> onRelease(CallBack f);
	[[nodiscard]] UIOConstructer<UIOButton> onPress(CallBack f);

	[[nodiscard]] UIOConstructer<UIOList> list(UIO::DIR listDir);

	template<class B>
	[[nodiscard]] UIOConstructer<UIOList> addToList(UIOConstructer<B> e);

	UniqueReference<UIOBase, T> get();
	T* operator->();

	template<class ...Args>
	static UIOConstructer<T> makeConstructer(Args&& ...args);

	UIOConstructer(UIOConstructer&& other) noexcept;
	UIOConstructer& operator=(UIOConstructer&& other);

	NOCOPY(UIOConstructer);
};

template<class T>
inline UIOConstructer<T>::UIOConstructer(UniqueReference<UIOBase, T> obj) {
	this->object = std::move(obj);
}

//--------------------------------------------------------------------------------
// pad
//--------------------------------------------------------------------------------

template<>
inline UIOConstructer<UIOPad> UIOConstructer<UIOPad>::pad(UIOSizeType padding) {
	auto ptr = this->object.get();
	ptr->bottom = padding;
	ptr->top = padding;
	ptr->left = padding;
	ptr->right = padding;
	return UIOConstructer<UIOPad>(std::move(this->object));
}

template<class T>
inline UIOConstructer<UIOPad> UIOConstructer<T>::pad(UIOSizeType padding) {
	auto refMan = Locator<ReferenceManager<UIOBase>>::get();
	return UIOConstructer<UIOPad>(refMan->makeUniqueRef<UIOPad>(std::move(object), padding));
}

//--------------------------------------------------------------------------------
// padTop
//--------------------------------------------------------------------------------

template<>
inline UIOConstructer<UIOPad> UIOConstructer<UIOPad>::padTop(UIOSizeType padding) {
	this->object.get()->top = padding;
	return UIOConstructer<UIOPad>(std::move(this->object));
}

template<class T>
inline UIOConstructer<UIOPad> UIOConstructer<T>::padTop(UIOSizeType padding) {
	auto pad = Locator<ReferenceManager<UIOBase>>::get()->makeUniqueRef<UIOPad>(std::move(this->object));
	pad.get()->top = padding;
	return UIOConstructer<UIOPad>(std::move(pad));
}

//--------------------------------------------------------------------------------
// padBottom
//--------------------------------------------------------------------------------

template<>
inline UIOConstructer<UIOPad> UIOConstructer<UIOPad>::padBottom(UIOSizeType padding) {
	this->object.get()->bottom = padding;
	return UIOConstructer<UIOPad>(std::move(this->object));
}

template<class T>
inline UIOConstructer<UIOPad> UIOConstructer<T>::padBottom(UIOSizeType padding) {
	auto pad = Locator<ReferenceManager<UIOBase>>::get()->makeUniqueRef<UIOPad>(std::move(this->object));
	pad.get()->bottom = padding;
	return UIOConstructer<UIOPad>(std::move(pad));
}

//--------------------------------------------------------------------------------
// padLeft
//--------------------------------------------------------------------------------

template<>
inline UIOConstructer<UIOPad> UIOConstructer<UIOPad>::padLeft(UIOSizeType padding) {
	this->object.get()->left = padding;
	return UIOConstructer<UIOPad>(std::move(this->object));
}

template<class T>
inline UIOConstructer<UIOPad> UIOConstructer<T>::padLeft(UIOSizeType padding) {
	auto pad = Locator<ReferenceManager<UIOBase>>::get()->makeUniqueRef<UIOPad>(std::move(this->object));
	pad.get()->left = padding;
	return UIOConstructer<UIOPad>(std::move(pad));
}

//--------------------------------------------------------------------------------
// padRight
//--------------------------------------------------------------------------------

template<>
inline UIOConstructer<UIOPad> UIOConstructer<UIOPad>::padRight(UIOSizeType padding) {
	this->object.get()->right = padding;
	return UIOConstructer<UIOPad>(std::move(this->object));
}

template<class T>
inline UIOConstructer<UIOPad> UIOConstructer<T>::padRight(UIOSizeType padding) {
	auto pad = Locator<ReferenceManager<UIOBase>>::get()->makeUniqueRef<UIOPad>(std::move(this->object));
	pad.get()->right = padding;
	return UIOConstructer<UIOPad>(std::move(pad));
}

//--------------------------------------------------------------------------------

template<class T>
inline UIOConstructer<T> UIOConstructer<T>::addBindCapture(std::function<void(T*)> f) {
	f(this->object.get());
	return std::move(*this);
}

template<class T>
inline UIOConstructer<T> UIOConstructer<T>::addFocussedBind(BindControl bindControl, CallBack callBack) {
	this->object.get()->addFocussedBind(bindControl, callBack);
	return std::move(*this);
}

template<class T>
inline UIOConstructer<T> UIOConstructer<T>::addActiveBind(BindControl bindControl, CallBack callBack) {
	this->object.get()->addActiveBind(bindControl, callBack);
	return std::move(*this);
}

template<class T>
inline UIOConstructer<T> UIOConstructer<T>::addGlobalBind(BindControl bindControl, CallBack callBack) {
	this->object.get()->addGlobalBind(bindControl, callBack);
	return std::move(*this);
}

template<class T>
inline UIOConstructer<T> UIOConstructer<T>::addOnHoverBind(BindControl bindControl, CallBack callBack) {
	this->object.get()->addOnHoverBind(bindControl, callBack);
	return std::move(*this);
}

template<class T>
inline UIOConstructer<T> UIOConstructer<T>::addGameWorldBind(BindControl bindControl, CallBack callBack) {
	this->object.get()->addGameWorldBind(bindControl, callBack);
	return std::move(*this);
}

//--------------------------------------------------------------------------------
// constrainHeight
//--------------------------------------------------------------------------------

template<>
inline UIOConstructer<UIOConstrainSize> UIOConstructer<UIOConstrainSize>::constrainHeight(UIOSizeType height) {
	this->object.get()->maybeHeight = height;
	return UIOConstructer<UIOConstrainSize>(std::move(this->object));
}

template<class T>
inline UIOConstructer<UIOConstrainSize> UIOConstructer<T>::constrainHeight(UIOSizeType height) {
	auto refMan = Locator<ReferenceManager<UIOBase>>::get();
	auto res = refMan->makeUniqueRef<UIOConstrainSize>(std::move(this->object));
	res.get()->maybeHeight = height;
	return UIOConstructer<UIOConstrainSize>(std::move(res));
}

//--------------------------------------------------------------------------------
// constrainWidth
//--------------------------------------------------------------------------------

template<>
inline UIOConstructer<UIOConstrainSize> UIOConstructer<UIOConstrainSize>::constrainWidth(UIOSizeType width) {
	this->object.get()->maybeWidth = width;
	return UIOConstructer<UIOConstrainSize>(std::move(this->object));
}

template<class T>
inline UIOConstructer<UIOConstrainSize> UIOConstructer<T>::constrainWidth(UIOSizeType width) {
	auto refMan = Locator<ReferenceManager<UIOBase>>::get();
	auto res = refMan->makeUniqueRef<UIOConstrainSize>(std::move(this->object));
	res.get()->maybeWidth = width;
	return UIOConstructer<UIOConstrainSize>(std::move(res));
}

//--------------------------------------------------------------------------------
// align
//--------------------------------------------------------------------------------

template<>
inline UIOConstructer<UIOConstrainSize> UIOConstructer<UIOConstrainSize>::align(UIO::ALIGNMENT align) {
	this->object.get()->alignment = align;
	return UIOConstructer<UIOConstrainSize>(std::move(this->object));
}

template<class T>
inline UIOConstructer<UIOConstrainSize> UIOConstructer<T>::align(UIO::ALIGNMENT align) {
	auto refMan = Locator<ReferenceManager<UIOBase>>::get();
	auto res = refMan->makeUniqueRef<UIOConstrainSize>(std::move(this->object));
	res.get()->alignment = align;
	return UIOConstructer<UIOConstrainSize>(std::move(res));
}

template<class T>
inline UIOConstructer<UIOConstrainSize> UIOConstructer<T>::alignCenter() {
	return this->align(UIO::ALIGNMENT::CENTER);
}

template<class T>
inline UIOConstructer<UIOConstrainSize> UIOConstructer<T>::alignTop() {
	return this->align(UIO::ALIGNMENT::TOP);
}

template<class T>
inline UIOConstructer<UIOConstrainSize> UIOConstructer<T>::alignBottom() {
	return this->align(UIO::ALIGNMENT::BOTTOM);
}

template<class T>
inline UIOConstructer<UIOConstrainSize> UIOConstructer<T>::alignLeft() {
	return this->align(UIO::ALIGNMENT::LEFT);
}

template<class T>
inline UIOConstructer<UIOConstrainSize> UIOConstructer<T>::alignRight() {
	return this->align(UIO::ALIGNMENT::RIGHT);
}

template<class T>
inline UIOConstructer<UIOConstrainSize> UIOConstructer<T>::alignBottomLeft() {
	return this->align(UIO::ALIGNMENT::BOTTOMLEFT);
}

template<class T>
inline UIOConstructer<UIOConstrainSize> UIOConstructer<T>::alignBottomRight() {
	return this->align(UIO::ALIGNMENT::BOTTOMRIGHT);
}

template<class T>
inline UIOConstructer<UIOConstrainSize> UIOConstructer<T>::alignTopLeft() {
	return this->align(UIO::ALIGNMENT::TOPLEFT);
}

template<class T>
inline UIOConstructer<UIOConstrainSize> UIOConstructer<T>::alignTopRight() {
	return this->align(UIO::ALIGNMENT::TOPRIGHT);
}

//--------------------------------------------------------------------------------

template<class T>
inline UIOConstructer<UIOColoredBackground> UIOConstructer<T>::background(glm::vec4 color) {
	auto refMan = Locator<ReferenceManager<UIOBase>>::get();
	return UIOConstructer<UIOColoredBackground>(refMan->makeUniqueRef<UIOColoredBackground>(color, std::move(this->object)));
}

template<class T>
inline UIOConstructer<UIOButton> UIOConstructer<T>::button(bool shrink) {
	return UIOConstructer<UIOButton>::makeConstructer(std::move(this->object), shrink);
}

template<class T>
inline UIOConstructer<UIOFreeSize> UIOConstructer<T>::window(std::string title, Rectangle size, int32_t types) {
	const int32_t resizeSliverSize = 7;

	auto refMan = Locator<ReferenceManager<UIOBase>>::get();

	auto mainPad = refMan->makeUniqueRef<UIOPad>(std::move(this->object));
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
			if (self->down) {
				windowPtr->moveTopLeftTo(params.uiState.getCursorPositionScreenClamped(0.99f) - self->mousePressOffset);
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
			if (self->down) {
				auto bottomRight = windowPtr->screenRectangle.getBottomRight();
				bottomRight.y =
					params.uiState.getCursorPositionScreenClamped(0.99f).y
					- self->mousePressOffset.y - self->screenRectangle.getAbsSize().y;
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
			if (self->down) {
				auto bottomRight = windowPtr->screenRectangle.getBottomRight();
				bottomRight.x =
					params.uiState.getCursorPositionScreenClamped(0.99f).x
					- self->mousePressOffset.x + self->screenRectangle.getAbsSize().x;
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
			if (self->down) {
				auto bottomRight =
					params.uiState.getCursorPositionScreenClamped(0.99f)
					- self->mousePressOffset + glm::vec2(1.0f, -1.0f) * self->screenRectangle.getAbsSize();
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

	return window;
}

template<class T>
inline UIOConstructer<UIOFreeSize> UIOConstructer<T>::free() {
	return UIOConstructer<UIOFreeSize>::makeConstructer(std::move(this->object));
}

template<class T>
inline UIOConstructer<UIOHideable> UIOConstructer<T>::hideable(bool focusOnShow) {
	return UIOConstructer<UIOHideable>::makeConstructer(std::move(this->object), focusOnShow);
}

//--------------------------------------------------------------------------------
// onRelease
//--------------------------------------------------------------------------------

template<>
inline UIOConstructer<UIOButton> UIOConstructer<UIOButton>::onRelease(CallBack f) {
	this->object.get()->setOnRelease(f);
	return std::move(*this);
}

template<class T>
inline UIOConstructer<UIOButton> UIOConstructer<T>::onRelease(CallBack f) {
	static_assert(0);
	return UIOConstructer<UIOButton>();
}

//--------------------------------------------------------------------------------
// onPress
//--------------------------------------------------------------------------------

template<>
inline UIOConstructer<UIOButton> UIOConstructer<UIOButton>::onPress(CallBack f) {
	this->object.get()->setOnPress(f);
	return std::move(*this);
}

template<class T>
inline UIOConstructer<UIOButton> UIOConstructer<T>::onPress(CallBack f) {
	static_assert(0);
	return UIOConstructer<UIOButton>();
}

template<class T>
inline UIOConstructer<UIOList> UIOConstructer<T>::list(UIO::DIR listDir) {
	auto res = UIOConstructer<UIOList>::makeConstructer(listDir);
	res->addElement(std::move(this->object));
	return res;
}

//--------------------------------------------------------------------------------
// addToList
//--------------------------------------------------------------------------------

template<>
template<class B>
inline UIOConstructer<UIOList> UIOConstructer<UIOList>::addToList(UIOConstructer<B> e) {
	(*this)->addElement(e.get());
	return std::move(*this);
}

template<class T>
template<class B>
inline UIOConstructer<UIOList> UIOConstructer<T>::addToList(UIOConstructer<B> e) {
	static_assert(0);
	return UIOConstructer<UIOList>();
}

//--------------------------------------------------------------------------------

template<class T>
inline UniqueReference<UIOBase, T> UIOConstructer<T>::get() {
	return std::move(this->object);
}

template<class T>
inline T* UIOConstructer<T>::operator->() {
	return this->object.get();
}

template<class T>
inline UIOConstructer<T>::UIOConstructer(UIOConstructer&& other) noexcept {
	this->object = std::move(other.object);
}

template<class T>
inline UIOConstructer<T>& UIOConstructer<T>::operator=(UIOConstructer&& other) {
	if (this != &other) {
		this->object = std::move(other.object);
	}
	return *this;
}

template<class T>
inline UIOConstructer<T> UIOConstructer<T>::addBind(std::function<void(UIOBase*)> f) {
	f(this->object.get());
	return std::move(*this);
}

template<class T>
inline UIOConstructer<T> UIOConstructer<T>::addBind(void(*f)(T* ptr)) {
	f(this->object.get());
	return std::move(*this);
}

template<class T>
template<class B>
inline UIOConstructer<T> UIOConstructer<T>::setPtr(B*& ptr) {
	static_assert(std::is_base_of<B, T>::value);
	ptr = static_cast<B*>(this->object.get());
	return std::move(*this);
}

template<class T>
template<class ...Args>
inline UIOConstructer<T> UIOConstructer<T>::makeConstructer(Args && ...args) {
	return UIOConstructer(Locator<ReferenceManager<UIOBase>>::get()->makeUniqueRef<T>(std::forward<Args>(args)...));
}
