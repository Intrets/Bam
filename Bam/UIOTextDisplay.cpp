#include "common.h"

#include "UIOTextDisplay.h"
#include "StringHelpers.h"
#include "RenderInfo.h"
#include "GameState.h"
#include "TextRenderer.h"
#include "UIOCallBackParams.h"
#include "TextRenderInfo.h"

UIOTextDisplay::UIOTextDisplay(Handle self) {
	selfHandle = self;
}

void UIOTextDisplay::setText(std::string text_) {
	split(0, text_, text, '\n', true);
}

ScreenRectangle UIOTextDisplay::updateSize(ScreenRectangle newScreenRectangle) {
	screenRectangle = newScreenRectangle;

	return screenRectangle;
}

int32_t UIOTextDisplay::addRenderInfo(GameState& gameState, RenderInfo& renderInfo, int32_t depth) {
	glm::vec4 color;
	glm::vec2 top = screenRectangle.getTopLeft();
	glm::vec2 bot;

	WindowTextRenderInfo textInfo(this->screenRectangle, Fonts::Font::ROBOTO_12, true);

	for (int32_t i = 0; i < 100; i++) {
		textInfo.addChar('u');
		textInfo.addChar('u');
		textInfo.addChar(' ');
		textInfo.addChar('r');
		textInfo.addChar('r');
		textInfo.newLine();
		textInfo.addString("!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\] ^ _`abcdefghijklmnopqrstuvwxyz{|}~");
	}


	renderInfo.textRenderInfo.windowTextRenderInfos.push_back(textInfo);

	// TODO: new text rendering

	//auto& cameraInfo = renderInfo.cameraInfo;
	//auto& textRenderer = *renderInfo.textRenderInfo.textRendererRef;

	//glm::vec2 fontSize2 = { 2 * fontSize / textRenderer.fontWidth,  2 * fontSize };
	//glm::vec2 fontScale = fontSize2 / glm::vec2(cameraInfo.x, cameraInfo.y);

	//if (this->text.empty()) {
	//	this->text.push_back("empty");
	//}

	//color = { 0.3,0.3,0.3,1.0 };
	//int32_t m = 0;
	//for (auto& t : text) {
	//	m = glm::max(m, static_cast<int32_t>(t.size()));
	//}
	//glm::vec2 s = glm::vec2(fontScale.x * m, -fontScale.y * text.size());
	//std::string test;
	//bot = top + s;
	//renderInfo.textRenderInfo.addTexts(
	//	*renderInfo.textRenderInfo.textRendererRef,
	//	renderInfo.cameraInfo,
	//	screenRectangle.getTopLeft(),
	//	screenRectangle.top.x - screenRectangle.bot.x,
	//	16,
	//	text,
	//	depth
	//);
	//renderInfo.uiRenderInfo.addRectangle(this->screenRectangle.bot, this->screenRectangle.top, glm::vec4(0.7f), depth);
	return depth + 1;
}
