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
#include "UIOShell.h"

template<class T>
class UIOConstructer
{
private:
	UniqueReference<UIOBase, T> object;

public:
	UIOConstructer(UniqueReference<UIOBase, T> obj);
	UIOConstructer() = default;
	~UIOConstructer() = default;

	UIOConstructer<T>& setPtr(T*& ptr);
	UIOConstructer<UIOPad> pad(UIOSizeType padding);
	UIOConstructer<UIOPad> padTop(UIOSizeType padding);
	UIOConstructer<UIOPad> padBottom(UIOSizeType padding);
	UIOConstructer<UIOPad> padLeft(UIOSizeType padding);
	UIOConstructer<UIOPad> padRight(UIOSizeType padding);

	UIOConstructer<T>& addBaseBind(std::function<void(UIOBase*)> f);
	UIOConstructer<T>& addBind(void (*f) (T* ptr));
	UIOConstructer<T>& addBindCapture(std::function<void(T*)> f);

	UIOConstructer<T>& addFocussedBind(BindControl bindControl, CallBack callBack);
	UIOConstructer<T>& addActiveBind(BindControl bindControl, CallBack callBack);
	UIOConstructer<T>& addGlobalBind(BindControl bindControl, CallBack callBack);
	UIOConstructer<T>& addOnHoverBind(BindControl bindControl, CallBack callBack);

	UIOConstructer<UIOConstrainSize> constrainHeight(UIOSizeType height);
	UIOConstructer<UIOConstrainSize> constrainWidth(UIOSizeType width);
	UIOConstructer<UIOConstrainSize> align(UIOConstrainSize::ALIGNMENT align);
	UIOConstructer<UIOColoredBackground> background(glm::vec4 color);
	UIOConstructer<UIOButton> button(bool shrink = false);
	UIOConstructer<UIOFreeSize> window(std::string title, Rectangle size, int32_t types);
	UIOConstructer<UIOFreeSize> free();

	UIOConstructer<UIOButton>& onRelease(CallBack f);
	UIOConstructer<UIOButton>& onPress(CallBack f);

	UniqueReference<UIOBase, T> get();

	template<class ...Args>
	static UIOConstructer<T> makeConstructer(Args&& ...args);

	UIOConstructer(UIOConstructer&& other);
	UIOConstructer& operator=(UIOConstructer&& other);

	NOCOPY(UIOConstructer);
};

template<class T>
inline UIOConstructer<T>::UIOConstructer(UniqueReference<UIOBase, T> obj) {
	this->object = std::move(obj);
}

template<class T>
inline UIOConstructer<T>& UIOConstructer<T>::setPtr(T*& ptr) {
	ptr = this->object.get();
	return *this;
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
inline UIOConstructer<T>& UIOConstructer<T>::addBindCapture(std::function<void(T*)> f) {
	f(this->object.get());
	return *this;
}

template<class T>
inline UIOConstructer<T>& UIOConstructer<T>::addFocussedBind(BindControl bindControl, CallBack callBack) {
	this->object.get()->addFocussedBind(bindControl, callBack);
	return *this;
}

template<class T>
inline UIOConstructer<T>& UIOConstructer<T>::addActiveBind(BindControl bindControl, CallBack callBack) {
	this->object.get()->addActiveBind(bindControl, callBack);
	return *this;
}

template<class T>
inline UIOConstructer<T>& UIOConstructer<T>::addGlobalBind(BindControl bindControl, CallBack callBack) {
	this->object.get()->addGlobalBind(bindControl, callBack);
	return *this;
}

template<class T>
inline UIOConstructer<T>& UIOConstructer<T>::addOnHoverBind(BindControl bindControl, CallBack callBack) {
	this->object.get()->addOnHoverBind(bindControl, callBack);
	return *this;
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
inline UIOConstructer<UIOConstrainSize> UIOConstructer<UIOConstrainSize>::align(UIOConstrainSize::ALIGNMENT align) {
	this->object.get()->alignment = align;
	return UIOConstructer<UIOConstrainSize>(std::move(this->object));
}

template<class T>
inline UIOConstructer<UIOConstrainSize> UIOConstructer<T>::align(UIOConstrainSize::ALIGNMENT align) {
	auto refMan = Locator<ReferenceManager<UIOBase>>::get();
	auto res = refMan->makeUniqueRef<UIOConstrainSize>(std::move(this->object));
	res.get()->alignment = align;
	return UIOConstructer<UIOConstrainSize>(std::move(res));
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
	auto refMan = Locator<ReferenceManager<UIOBase>>::get();

	auto mainPad = refMan->makeUniqueRef<UIOPad>(std::move(this->object));
	auto mainPadPtr = mainPad.get();
	mainPadPtr->top = { UIOSizeType::PX, 20 };
	if (types & UIOWindow::TYPE::RESIZE) {
		types |= UIOWindow::TYPE::RESIZEHORIZONTAL | UIOWindow::TYPE::RESIZEVERTICAL;
	}
	if (types & UIOWindow::TYPE::RESIZEHORIZONTAL) {
		mainPadPtr->right = { UIOSizeType::PX, 10 };
	}
	if (types & UIOWindow::TYPE::RESIZEVERTICAL) {
		mainPadPtr->bottom = { UIOSizeType::PX, 10 };
	}

	UIOWindow* windowPtr;
	auto window = UIOConstructer<UIOWindow>::makeConstructer(std::move(mainPad))
		.setPtr(windowPtr)
		.free();
	windowPtr->screenRectangle.set(size);

	UIOList* topList;
	auto topBar = UIOConstructer<UIOList>::makeConstructer(UIOList::DIR::LEFT)
		.setPtr(topList)
		.constrainHeight(UIOSizeType(UIOSizeType::PX, 20))
		.align(UIOConstrainSize::ALIGNMENT::TOP)
		.get();

	windowPtr->topBarPtr = topBar.get();
	windowPtr->addElement(std::move(topBar));

	if (types & UIOWindow::TYPE::CLOSE) {
		auto close = TextConstructer::constructSingleLineDisplayText(" x")
			.align(UIOConstrainSize::ALIGNMENT::CENTER)
			.button()
			.onRelease([](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
		{
			return BIND_RESULT::CLOSE;
		})
			.pad(UIOSizeType(UIOSizeType::PX, 1))
			.constrainHeight(UIOSizeType(UIOSizeType::PX, 20))
			.constrainWidth(UIOSizeType(UIOSizeType::PX, 20))
			.get();

		topList->addElement(std::move(close));
	}

	if (types & UIOWindow::TYPE::MINIMISE) {
		auto close = TextConstructer::constructSingleLineDisplayText(" _")
			.align(UIOConstrainSize::ALIGNMENT::CENTER)
			.button()
			.onRelease([windowPtr](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
		{
			windowPtr->minimized = !windowPtr->minimized;
			return BIND_RESULT::CONTINUE;
		})
			.pad(UIOSizeType(UIOSizeType::PX, 1))
			.constrainHeight(UIOSizeType(UIOSizeType::PX, 20))
			.constrainWidth(UIOSizeType(UIOSizeType::PX, 20))
			.get();

		topList->addElement(std::move(close));
	}

	UIOButton* titleBarPtr;
	if (types & UIOWindow::TYPE::MOVE) {
		auto titleBar = TextConstructer::constructSingleLineDisplayText(title)
			.button()
			.addFocussedBind(
				{ ControlState::CONTROLS::MOUSE_POS_CHANGED, ControlState::CONTROLSTATE_PRESSED },
				[windowPtr](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
		{
			auto self = static_cast<UIOButton*>(self_);
			if (self->down) {
				windowPtr->moveTopLeftTo(params.uiState.getCursorPositionScreenClamped(0.99f) - self->mousePressOffset);
			}
			return BIND_RESULT::CONTINUE;
		})
			.setPtr(titleBarPtr)
			.constrainHeight(UIOSizeType(UIOSizeType::PX, 20))
			.get();
		topList->addElement(std::move(titleBar));
	}
	else {
		auto titleBar = TextConstructer::constructSingleLineDisplayText(title, false)
			.background(COLORS::UI::FOREGROUND)
			.constrainHeight(UIOSizeType(UIOSizeType::PX, 20))
			.get();
		topList->addElement(std::move(titleBar));
	}

	if (types & UIOWindow::TYPE::RESIZEVERTICAL) {
		auto bottomBar = UIOConstructer<UIOButton>::makeConstructer()
			.addFocussedBind(
				{ ControlState::CONTROLS::MOUSE_POS_CHANGED, ControlState::CONTROLSTATE_PRESSED },
				[windowPtr](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
		{
			auto self = static_cast<UIOButton*>(self_);
			if (self->down) {
				auto bottomRight = windowPtr->screenRectangle.getBottomRight();
				bottomRight.y =
					params.uiState.getCursorPositionScreenClamped(0.99f).y
					- self->mousePressOffset.y - self->screenRectangle.size().y;
				if (windowPtr->screenRectangle.top.y - bottomRight.y < 0.2f) {
					bottomRight.y = windowPtr->screenRectangle.top.y - 0.2f;
				}
				windowPtr->screenRectangle.setBottomRight(bottomRight);
				windowPtr->updateSize(windowPtr->screenRectangle);
			}
			return BIND_RESULT::CONTINUE;
		})
			.padLeft(UIOSizeType(UIOSizeType::PX, 1))
			.padTop(UIOSizeType(UIOSizeType::PX, 1))
			.padRight(UIOSizeType(UIOSizeType::PX, types & UIOWindow::TYPE::RESIZEHORIZONTAL ? 10 : 0))
			.constrainHeight(UIOSizeType(UIOSizeType::PX, 10))
			.align(UIOConstrainSize::ALIGNMENT::BOTTOMLEFT)
			.get();

		windowPtr->addElement(std::move(bottomBar));
	}

	if (types & UIOWindow::TYPE::RESIZEHORIZONTAL) {
		auto rightBar = UIOConstructer<UIOButton>::makeConstructer()
			.addFocussedBind(
				{ ControlState::CONTROLS::MOUSE_POS_CHANGED, ControlState::CONTROLSTATE_PRESSED },
				[windowPtr](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
		{
			auto self = static_cast<UIOButton*>(self_);
			if (self->down) {
				auto bottomRight = windowPtr->screenRectangle.getBottomRight();
				bottomRight.x =
					params.uiState.getCursorPositionScreenClamped(0.99f).x
					- self->mousePressOffset.x + self->screenRectangle.size().x;
				if (bottomRight.x - windowPtr->screenRectangle.bot.x < 0.2f) {
					bottomRight.x = windowPtr->screenRectangle.bot.x + 0.2f;
				}
				windowPtr->screenRectangle.setBottomRight(bottomRight);
				windowPtr->updateSize(windowPtr->screenRectangle);
			}
			return BIND_RESULT::CONTINUE;
		})
			.padLeft(UIOSizeType(UIOSizeType::PX, 1))
			.padTop(UIOSizeType(UIOSizeType::PX, 20))
			.padBottom(UIOSizeType(UIOSizeType::PX, types & UIOWindow::TYPE::RESIZEVERTICAL ? 10 : 0))
			.constrainWidth(UIOSizeType(UIOSizeType::PX, 10))
			.align(UIOConstrainSize::ALIGNMENT::RIGHT)
			.get();

		windowPtr->addElement(std::move(rightBar));
	}

	if (types & UIOWindow::TYPE::RESIZE) {
		auto cornerBar = UIOConstructer<UIOButton>::makeConstructer()
			.addFocussedBind(
				{ ControlState::CONTROLS::MOUSE_POS_CHANGED, ControlState::CONTROLSTATE_PRESSED },
				[windowPtr](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
		{
			auto self = static_cast<UIOButton*>(self_);
			if (self->down) {
				auto bottomRight =
					params.uiState.getCursorPositionScreenClamped(0.99f)
					- self->mousePressOffset + glm::vec2(1.0f, -1.0f) * self->screenRectangle.size();
				if (bottomRight.x - windowPtr->screenRectangle.bot.x < 0.2f) {
					bottomRight.x = windowPtr->screenRectangle.bot.x + 0.2f;
				}
				if (windowPtr->screenRectangle.top.y - bottomRight.y < 0.2f) {
					bottomRight.y = windowPtr->screenRectangle.top.y - 0.2f;
				}
				windowPtr->screenRectangle.setBottomRight(bottomRight);
				windowPtr->updateSize(windowPtr->screenRectangle);
			}
			return BIND_RESULT::CONTINUE;
		})
			.constrainWidth(UIOSizeType(UIOSizeType::PX, 10))
			.constrainHeight(UIOSizeType(UIOSizeType::PX, 10))
			.align(UIOConstrainSize::ALIGNMENT::BOTTOMRIGHT)
			.get();

		windowPtr->addElement(std::move(cornerBar));
	}

	return std::move(window);
}

template<class T>
inline UIOConstructer<UIOFreeSize> UIOConstructer<T>::free() {
	return UIOConstructer<UIOFreeSize>::makeConstructer(std::move(this->object));
}

//--------------------------------------------------------------------------------
// onRelease
//--------------------------------------------------------------------------------

template<>
inline UIOConstructer<UIOButton>& UIOConstructer<UIOButton>::onRelease(CallBack f) {
	this->object.get()->onRelease = f;
	return *this;
}

template<class T>
inline UIOConstructer<UIOButton>& UIOConstructer<T>::onRelease(CallBack f) {
	static_assert(0);
	return UIOConstructer<UIOButton>();
}

//--------------------------------------------------------------------------------
// onPress
//--------------------------------------------------------------------------------

template<>
inline UIOConstructer<UIOButton>& UIOConstructer<UIOButton>::onPress(CallBack f) {
	this->object.get()->onPress = f;
	return *this;
}

template<class T>
inline UIOConstructer<UIOButton>& UIOConstructer<T>::onPress(CallBack f) {
	static_assert(0);
	return UIOConstructer<UIOButton>();
}

//--------------------------------------------------------------------------------

template<class T>
inline UniqueReference<UIOBase, T> UIOConstructer<T>::get() {
	return std::move(this->object);
}

template<class T>
inline UIOConstructer<T>::UIOConstructer(UIOConstructer&& other) {
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
inline UIOConstructer<T>& UIOConstructer<T>::addBaseBind(std::function<void(UIOBase*)> f) {
	f(this->object.get());
	return *this;
}

template<class T>
inline UIOConstructer<T>& UIOConstructer<T>::addBind(void(*f)(T* ptr)) {
	f(this->object.get());
	return *this;
}

template<class T>
template<class ...Args>
inline UIOConstructer<T> UIOConstructer<T>::makeConstructer(Args && ...args) {
	return UIOConstructer(Locator<ReferenceManager<UIOBase>>::get()->makeUniqueRef<T>(std::forward<Args>(args)...));
}
