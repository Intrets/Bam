#pragma once

#include "UIOBase.h"
#include "UIOTextDisplay.h"
#include "UIOConstructer2.h"
#include "UIOBinds.h"
#include "UIOConstrainSize.h"
#include "Colors.h"
#include "UIOColoredBackground.h"
#include "UIOPad.h"
#include "UIOButton.h"
#include "UIOCallBackParams.h"

class UIOTextDisplay;

template<class T>
class UIODropDownList : public UIOBaseMulti
{
private:
	UIOList* buttonList;
	std::vector<UIOButton*> buttons;
	UIOBase* expanded;

	bool validView = true;

	std::function<std::string(T const&)> display;

public:
	UIODropDownList(Handle self, std::function<std::string(T const&)> f);

	int32_t selected = 0;
	bool isExpanded = true;

	UIOButton* single;
	UIOTextDisplay* singleText;

	std::vector<T> list;

	std::optional<T const*> getSelected();

	void setList(std::vector<T> const& l);

	virtual CallBackBindResult runGlobalBinds(State& state) override;
	virtual CallBackBindResult runFocussedBinds(State& state) override;
	virtual CallBackBindResult runOnHoverBinds(State& state) override;
	virtual CallBackBindResult runActiveBinds(State& state) override;
	virtual CallBackBindResult runGameWorldBinds(State& state) override;

	virtual int32_t addRenderInfo(GameState& gameState, RenderInfo& renderInfo, int32_t depth) override;
};

template<class T>
inline UIODropDownList<T>::UIODropDownList(Handle self, std::function<std::string(T const&)> f) {
	this->display = f;

	UIO2::Global::push();
	UIO2::alignTopLeft();
	UIO2::constrainHeight({ UIO::SIZETYPE::FH, 1.2f });
	auto [b, s] = UIO2::textButton2("");

	this->single = b;
	this->singleText = s;

	b->setOnRelease([this](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
	{
		this->isExpanded = true;
		return BIND::RESULT::CONTINUE;
	});

	this->addElement(UIO2::Global::pop());

	UIO2::Global::push();

	UIO2::alignTopLeft();
	this->expanded = UIO2::background(COLORS::UI::FOREGROUND);

	this->buttonList = UIO2::startList(UIO::DIR::DOWN);
	this->buttonList->addGlobalBind({ CONTROL::KEY::ACTION0 }, [this](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
	{
		if (!self_->getScreenRectangle().contains(params.uiState.getCursorPositionScreen())) {
			this->isExpanded = false;
		}
		return BIND::RESULT::CONTINUE;
	});

	this->buttonList->addGlobalBind({ CONTROL::KEY::ANYTHING_KEY }, [this](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
	{
		if (!self_->getScreenRectangle().contains(params.uiState.getCursorPositionScreen())) {
			this->isExpanded = false;
		}
		return BIND::RESULT::CONTINUE;
	});

	UIO2::endList();

	this->addElement(UIO2::Global::pop());
}

template<class T>
inline std::optional<T const*> UIODropDownList<T>::getSelected() {
	if (indexInVector(this->selected, this->list)) {
		return &this->list[selected];
	}
	else {
		return std::nullopt;
	}
}

template<class T>
inline void UIODropDownList<T>::setList(std::vector<T> const& l) {
	this->list = l;

	this->buttonList->clear();
	this->buttons.clear();

	int32_t index = 0;
	for (auto& element : this->list) {
		UIO2::Global::push();

		UIO2::constrainHeight({ UIO::SIZETYPE::FH, 1.2f });
		this->buttons.push_back(UIO2::textButton(this->display(element)));
		this->buttons[index]->setOnPress([this, index](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
		{
			this->selected = index;
			this->isExpanded = false;
			this->singleText->setText(this->display(this->list[index]));
			this->buttons[index]->dePress();
			return BIND::RESULT::CONTINUE;
		});

		this->buttonList->addElement(UIO2::Global::pop());

		++index;
	}
}

template<class T>
inline CallBackBindResult UIODropDownList<T>::runGlobalBinds(State& state) {
	CallBackBindResult sumResult = 0;

	if (this->isExpanded) {
		sumResult = this->expanded->runGlobalBinds(state);
	}
	else {
		sumResult = this->single->runGlobalBinds(state);
	}

	if (sumResult & BIND::RESULT::STOP) {
		return sumResult;
	}

	return sumResult | this->UIOBase::runGlobalBinds(state);
}

template<class T>
inline CallBackBindResult UIODropDownList<T>::runFocussedBinds(State& state) {
	CallBackBindResult sumResult = 0;

	if (this->isExpanded) {
		sumResult = this->expanded->runFocussedBinds(state);
	}
	else {
		sumResult = this->single->runFocussedBinds(state);
	}

	if (sumResult & BIND::RESULT::STOP) {
		return sumResult;
	}

	return sumResult | this->UIOBase::runFocussedBinds(state);
}

template<class T>
inline CallBackBindResult UIODropDownList<T>::runOnHoverBinds(State& state) {
	CallBackBindResult sumResult = 0;

	if (this->isExpanded) {
		sumResult = this->expanded->runOnHoverBinds(state);
	}
	else {
		sumResult = this->single->runOnHoverBinds(state);
	}

	if (sumResult & BIND::RESULT::STOP) {
		return sumResult;
	}

	return sumResult | this->UIOBase::runOnHoverBinds(state);
}

template<class T>
inline CallBackBindResult UIODropDownList<T>::runActiveBinds(State& state) {
	CallBackBindResult sumResult = 0;

	if (this->isExpanded) {
		sumResult = this->expanded->runActiveBinds(state);
	}
	else {
		sumResult = this->single->runActiveBinds(state);
	}

	if (sumResult & BIND::RESULT::STOP) {
		return sumResult;
	}

	return sumResult | this->UIOBase::runActiveBinds(state);
}

template<class T>
inline CallBackBindResult UIODropDownList<T>::runGameWorldBinds(State& state) {
	CallBackBindResult sumResult = 0;

	if (this->isExpanded) {
		sumResult = this->expanded->runGameWorldBinds(state);
	}
	else {
		sumResult = this->single->runGameWorldBinds(state);
	}

	if (sumResult & BIND::RESULT::STOP) {
		return sumResult;
	}

	return sumResult | this->UIOBase::runGameWorldBinds(state);
}

template<class T>
inline int32_t UIODropDownList<T>::addRenderInfo(GameState& gameState, RenderInfo& renderInfo, int32_t depth) {
	if (this->isExpanded) {
		depth = this->expanded->addRenderInfo(gameState, renderInfo, depth);
	}
	else {
		depth = this->single->addRenderInfo(gameState, renderInfo, depth);
	}

	return 1 + depth;
}
