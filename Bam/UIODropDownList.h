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
#include "UIOAnchoredProxy.h"
#include "UIState.h"

class UIOTextDisplay;

template<class T>
class UIODropDownList : public UIOBaseMulti
{
private:
	std::function<std::string(T const&)> display;

public:
	UIOAnchoredProxy* proxy;
	int32_t selected = 0;
	std::vector<T> list;

	UIOTextDisplay* buttonText;

	UIODropDownList(Handle self, std::function<std::string(T const&)> f);
	virtual ~UIODropDownList() = default;

	std::optional<T const*> getSelected();
	bool select(int32_t index);

	void spawnPopUpList(UIState& uiState);

	void setList(std::vector<T> const& l);
};

template<class T>
inline void UIODropDownList<T>::spawnPopUpList(UIState& uiState) {
	UIO2::Global::push();

	UIO2::padTop({ UIO::SIZETYPE::PX, 3 });
	UIO2::padRight({ UIO::SIZETYPE::PX, 3 });
	UIO2::padLeft({ UIO::SIZETYPE::PX, 3 });
	UIO2::background(COLORS::DARKEN2(COLORS::UI::BACKGROUND));
	UIO2::startList(UIO::DIR::DOWN);

	int32_t index = 0;
	for (auto& element : this->list) {

		UIO2::constrainHeight({ UIO::SIZETYPE::FH, 1.2f });
		auto button = UIO2::textButton(this->display(element));
		button->setOnPress([index, ref = ManagedReference<UIOBase, UIODropDownList<T>>(this->getSelfHandle())](UIOCallBackParams& params, UIOBase* self_)->CallBackBindResult
		{
			if (ref.isValid()) {
				ref.get()->select(index);
			}
			return BIND::RESULT::CLOSE;
		});

		++index;
	}

	UIO2::endList();

	this->proxy->setProxy(std::move(UIO2::Global::pop()), uiState);
}

template<class T>
inline UIODropDownList<T>::UIODropDownList(Handle self, std::function<std::string(T const&)> f) {
	this->selfHandle = self;
	this->display = f;

	UIO2::Global::push();
	UIO2::alignTopLeft();
	UIO2::constrainHeight({ UIO::SIZETYPE::FH, 1.2f });

	auto [button, text] = UIO2::textButton2("");
	this->buttonText = text;

	button->setOnRelease([this](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult
	{
		this->spawnPopUpList(params.uiState);
		return BIND::RESULT::CONTINUE;
	});

	this->addElement(UIO2::Global::pop());

	UIO2::Global::push();

	this->proxy = UIO2::makeEnd<UIOAnchoredProxy>();

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
inline bool UIODropDownList<T>::select(int32_t index) {
	if (!indexInVector(index, this->list)) {
		return false;
	}
	else {
		this->selected = index;
		this->buttonText->setText(this->display(this->list[this->selected]));
		return true;
	}
}

template<class T>
inline void UIODropDownList<T>::setList(std::vector<T> const& l) {
	this->list = l;
}

