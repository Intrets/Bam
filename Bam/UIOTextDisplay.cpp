#include "common.h"

#include "UIOTextDisplay.h"
#include "StringHelpers.h"
#include "RenderInfo.h"
#include "GameState.h"
#include "TextRenderer.h"
#include "UIOCallBackParams.h"
#include "TextRenderInfo.h"

UIOTextDisplay::UIOTextDisplay(Handle self) {
	this->selfHandle = self;
}

void UIOTextDisplay::setText(std::string text_) {
	split(0, text_, this->text, '\n', true);
	this->cachedText = std::nullopt;
}

void UIOTextDisplay::addLine(std::string text_) {
	text.push_back(text_);
	this->cachedText = std::nullopt;
}

ScreenRectangle UIOTextDisplay::updateSize(ScreenRectangle newScreenRectangle) {
	if (!newScreenRectangle.equals(this->screenRectangle)) {
		this->cachedText = std::nullopt;
	}
	this->screenRectangle = newScreenRectangle;

	return this->screenRectangle;
}

int32_t UIOTextDisplay::addRenderInfo(GameState& gameState, RenderInfo& renderInfo, int32_t depth) {
	glm::vec4 color;
	glm::vec2 top = this->screenRectangle.getTopLeft();
	glm::vec2 bot;

	if (!this->cachedText.has_value()) {
		WindowTextRenderInfo textInfo(this->screenRectangle, Fonts::Font::ROBOTO_12, true);

		for (auto& line : this->text) {
			textInfo.addString(line);
			textInfo.newLine();
		}

		this->cachedText = textInfo;
	}

	this->cachedText.value().setDepth(depth++);
	renderInfo.textRenderInfo.windowTextRenderInfos.push_back(this->cachedText.value());

	renderInfo.uiRenderInfo.addRectangle(this->screenRectangle.bot, this->screenRectangle.top, glm::vec4(0.9f, 0.9f, 0.9f, 1.0f), depth++);

	return depth;
}
