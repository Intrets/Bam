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
#include "PlayerState.h"
#include "UIOAnchoredProxy.h"
#include "UIState.h"

class UIOTextDisplay;

template<class T>
class UIODropDownList : public UIOBaseMulti
{
private:
	std::function<std::string(T const&)> display;

	std::optional<std::function<std::vector<T>(PlayerState&)>> listGenerator;

public:
	WeakReference<UIOBase, UIOAnchoredProxy> proxy;
	int32_t selected = -1;
	std::vector<T> list;

	WeakReference<UIOBase, UIOTextDisplay> buttonText;

	UIODropDownList(Handle self, std::function<std::string(T const&)> f);
	virtual ~UIODropDownList() = default;

	std::optional<T const*> getSelected();
	bool select(int32_t index);

	void spawnPopUpList(PlayerState& playerState);

	void setList(std::vector<T> const& l);
	void setList(std::function<std::vector<T>(PlayerState&)> f);
};

template<class T>
inline void UIODropDownList<T>::spawnPopUpList(PlayerState& playerState) {
	if (this->listGenerator) {
		this->list = this->listGenerator.value()(playerState);
	}

	UIO2::Global::push();

	UIO2::padTop({ UIO::SIZETYPE::PX, 3 });
	UIO2::padRight({ UIO::SIZETYPE::PX, 3 });
	UIO2::padLeft({ UIO::SIZETYPE::PX, 3 });
	UIO2::background(COLORS::DARKEN2(COLORS::UI::BACKGROUND));

	UIO2::startList(UIO::DIR::DOWN);

	if (this->list.size() == 0) {
		UIO2::constrainHeight({ UIO::SIZETYPE::FH, 1.2f });
		auto button = UIO2::textButton("nothing");
		button.get()->setColor(COLORS::UI::RED);
		button.get()->setOnPress([](PlayerState& playerState, UIOBase* self_) -> CallBackBindResult
		{
			return BIND::RESULT::CLOSE;
		});
	}
	else {
		int32_t index = 0;
		for (auto& element : this->list) {
			UIO2::constrainHeight({ UIO::SIZETYPE::FH, 1.2f });
			auto button = UIO2::textButton(this->display(element));
			auto& manager = UIO2::Global::getManager();
			button.get()->setOnPress([index, ref = ManagedReference<UIOBase, UIODropDownList<T>>(manager, this)](PlayerState& playerState, UIOBase* self_)->CallBackBindResult
			{
				if (auto r = ref.getRef()) {
					r.get()->select(index);
				}
				return BIND::RESULT::CLOSE;
			});

			++index;
		}
	}
	UIO2::endList();

	this->proxy.get()->setProxy(std::move(UIO2::Global::pop()), playerState.uiState);
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

	button.get()->setOnRelease([this](PlayerState& playerState, UIOBase* self_) -> CallBackBindResult
	{
		this->spawnPopUpList(playerState);
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
		this->buttonText.get()->setText(this->display(this->list[this->selected]));
		return true;
	}
}

template<class T>
inline void UIODropDownList<T>::setList(std::vector<T> const& l) {
	assert(!this->listGenerator.has_value());
	this->list = l;
}

template<class T>
inline void UIODropDownList<T>::setList(std::function<std::vector<T>(PlayerState&)> f) {
	this->listGenerator = f;
}

