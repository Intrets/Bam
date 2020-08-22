#pragma once

#include "UIOBase.h"
#include "UIOTextDisplay.h"
#include "UIOTextConstructers.h"
#include "UIOBinds.h"
#include "RenderInfo.h"
#include "Colors.h"

template<class T>
class UIOListSelection : public UIOBase
{
private:
	int32_t selection;
	UIOTextDisplay* textDisplay;

	std::vector<T> list;

	std::function<std::string(T const&)> display;

public:
	UIOListSelection(Handle self);
	UIOListSelection(Handle self, std::function<std::string(T const&)> f);

	std::optional<const T&> getSelected();
	void setList(std::vector<T>& l);

	virtual ScreenRectangle updateSize(ScreenRectangle newScreenRectangle) override;
	virtual int32_t addRenderInfo(GameState& gameState, RenderInfo& renderInfo, int32_t depth) override;
};

template<class T>
std::optional<const T&> UIOListSelection<T>::getSelected() {
	if (indexInVector(this->selection, this->list)) {
		return this->list[this->selection];
	}
	else {
		return std::nullopt;
	}
}

template<class T>
void UIOListSelection<T>::setList(std::vector<T>& l) {
	this->list = l;
	this->textDisplay->text.empty();
	for (const auto& e : this->list) {
		this->textDisplay->text.addLine(this->display(e));
	}
}

template<class T>
UIOListSelection<T>::UIOListSelection(Handle self) {
	display = [](const T&)
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
	depth = UIOBase::addRenderInfo(gameState, renderInfo, depth);

	if (this->textDisplay->text.cachedRenderInfo.has_value()) {
		auto index = this->textDisplay->text.cursorIndex;

		auto maybeCursorShape = this->textDisplay->text.cachedRenderInfo.value().getCursorPos(index);
		if (maybeCursorShape.has_value()) {
			glm::vec4 cursorShape = maybeCursorShape.value();

			glm::vec2 a = (glm::vec2(cursorShape[0], cursorShape[1]) - this->textDisplay->text.view) / 2.0f + 0.5f;
			glm::vec2 b = glm::vec2(cursorShape[2], cursorShape[3]) / 2.0f;
			a *= this->screenRectangle.size();
			b *= this->screenRectangle.size();

			a += this->screenRectangle.getBottomLeft();
			b += a;

			a.y = glm::max(this->screenRectangle.bot.y, a.y);
			b.y = glm::min(this->screenRectangle.top.y, b.y);

			if (a.y < b.y) {
				glm::vec2 bot = { this->screenRectangle.bot.x, a.y };
				glm::vec2 top = { this->screenRectangle.top.x, b.y };

				renderInfo.uiRenderInfo.addRectangle(bot, top, COLORS::UI::CURSOR, depth++);
			}
		}
	}

	return depth;

}
