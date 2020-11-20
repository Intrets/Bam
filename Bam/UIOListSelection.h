#pragma once

#include "UIOBase.h"
#include "UIOTextDisplay.h"
#include "UIOTextConstructers.h"
#include "UIOBinds.h"
#include "RenderInfo.h"
#include "Colors.h"

template<class T>
class UIOListSelection : public UIOBaseSingle
{
private:
	UIOTextDisplay* textDisplay;

	std::vector<T> list;

	bool validView = true;
	std::function<std::string(T const&)> display;

public:
	UIOListSelection(Handle self);
	UIOListSelection(Handle self, std::function<std::string(T const&)> f);

	void setSelected(int32_t index);
	std::optional<T*> getSelected();
	void setList(std::vector<T> const& l);
	std::vector<T> const& getList();

	void invalidateView();

	virtual ScreenRectangle updateSize(ScreenRectangle newScreenRectangle) override;
	virtual int32_t addRenderInfo(GameState& gameState, RenderInfo& renderInfo, int32_t depth) override;

};

template<class T>
inline void UIOListSelection<T>::setSelected(int32_t index) {
	this->textDisplay->setCursor(glm::ivec2(0, index));
}

template<class T>
std::optional<T*> UIOListSelection<T>::getSelected() {
	auto selection = this->textDisplay->text.getCursor().y;
	if (indexInVector(selection, this->list)) {
		return &this->list[selection];
	}
	else {
		return std::nullopt;
	}
}

template<class T>
void UIOListSelection<T>::setList(std::vector<T> const& l) {
	this->list = l;
	this->validView = false;
}

template<class T>
inline std::vector<T> const& UIOListSelection<T>::getList() {
	return this->list;
}

template<class T>
inline void UIOListSelection<T>::invalidateView() {
	this->validView = false;
}

template<class T>
UIOListSelection<T>::UIOListSelection(Handle self) {
	this->display = [](const T&)
	{
		return "";
	};

	this->selfHandle = self;
	this->addElement(
		TextConstructer::constructDisplayText("")
		.setPtr(this->textDisplay)
		//.addBaseBind(UIOBinds::Base::activatable)
		.addBind(UIOBinds::TextEdit::clickSelect)
		.get()
	);
}

template<class T>
UIOListSelection<T>::UIOListSelection(Handle self, std::function<std::string(T const&)> f) :
	UIOListSelection(self) {
	this->display = f;
}

template<class T>
ScreenRectangle UIOListSelection<T>::updateSize(ScreenRectangle newScreenRectangle) {
	this->textDisplay->updateSize(newScreenRectangle);
	this->screenRectangle = newScreenRectangle;
	return this->screenRectangle;
}

template<class T>
inline int32_t UIOListSelection<T>::addRenderInfo(GameState& gameState, RenderInfo& renderInfo, int32_t depth) {
	if (!this->validView) {
		this->textDisplay->text.empty();
		for (const auto& e : this->list) {
			this->textDisplay->text.addLine(this->display(e));
		}
		this->validView = true;
	}

	depth = UIOBaseSingle::addRenderInfo(gameState, renderInfo, depth);

	if (auto const& maybeCursorQuad = this->textDisplay->text.getCursorQuadScreen()) {
		auto const& cursorQuad = maybeCursorQuad.value();

		float a = glm::max(this->screenRectangle.getBot(), cursorQuad.getBot());
		float b = glm::min(this->screenRectangle.getTop(), cursorQuad.getTop());

		if (a < b) {
			glm::vec2 bot = { this->screenRectangle.getLeft(), a };
			glm::vec2 top = { this->screenRectangle.getRight(), b };

			renderInfo.uiRenderInfo.addRectangle(bot, top, COLORS::UI::CURSOR, depth++);
		}
	}

	return depth;
}
