#include "common.h"

#include "UIOTextDisplay.h"
#include "StringHelpers.h"
#include "RenderInfo.h"
#include "GameState.h"
#include "TextRenderer.h"
#include "UIOCallBackParams.h"

UIOTextDisplay::UIOTextDisplay(Handle self) {
	selfHandle = self;

	addGlobalBind({ CONTROLS::MOUSE_POS_CHANGED, CONTROLSTATE::CONTROLSTATE_PRESSED }, [&](UIOCallBackParams& state, UIOBase* self_) -> CallBackBindResult {
		this->hover = this->screenRectangle.contains(state.uiState.getCursorPositionScreen());
		return BIND_RESULT::CONTINUE;
	});
}

void UIOTextDisplay::setText(std::string text_) {
	split(0, text_, text, '\n', true);
}

ScreenRectangle UIOTextDisplay::updateSize(ScreenRectangle newScreenRectangle) {
	float fontHeight = 2 * static_cast<float>(fontSize) / newScreenRectangle.screenPixels.y;

	screenRectangle = newScreenRectangle;

	screenRectangle.setHeight(fontHeight);

	return screenRectangle;
}

int32_t UIOTextDisplay::addRenderInfo(GameState& gameState, RenderInfo& renderInfo, int32_t depth) {
	glm::vec4 color;
	glm::vec2 top = screenRectangle.getTopLeft();
	glm::vec2 bot;

	auto& cameraInfo = renderInfo.cameraInfo;
	auto& textRenderer = *renderInfo.textRenderInfo.textRendererRef;

	glm::vec2 fontSize2 = { 2 * fontSize / textRenderer.fontWidth,  2 * fontSize };
	glm::vec2 fontScale = fontSize2 / glm::vec2(cameraInfo.x, cameraInfo.y);

	if (this->text.empty()) {
		this->text.push_back("empty");
	}

	color = { 0.3,0.3,0.3,1.0 };
	int32_t m = 0;
	for (auto& t : text) {
		m = glm::max(m, static_cast<int32_t>(t.size()));
	}
	glm::vec2 s = glm::vec2(fontScale.x * m, -fontScale.y * text.size());
	std::string test;
	bot = top + s;
	renderInfo.textRenderInfo.addTexts(
		*renderInfo.textRenderInfo.textRendererRef,
		renderInfo.cameraInfo,
		screenRectangle.getTopLeft(),
		screenRectangle.top.x - screenRectangle.bot.x,
		16,
		text,
		depth
	);
	renderInfo.uiRenderInfo.addRectangle(bot, top, color, depth);
	return depth + 1;
}
