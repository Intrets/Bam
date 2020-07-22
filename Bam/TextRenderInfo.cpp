#include "common.h"

#include "TextRenderInfo.h"
#include "TextRenderer.h"
#include "CameraInfo.h"
#include "StringHelpers.h"
#include "Fonts.h"

//void TextRenderInfo::addChar(glm::vec2 offset, char c, glm::vec2 scale, int32_t depth) {
//	chars.push_back(c);
//	offsets.push_back(glm::vec3(offset, static_cast<float>(depth) / 1000.0f));
//	scales.push_back(scale);
//	//TODO: add function argument for font type
//	//fonts.push_back(static_cast<int32_t>(c + 1) % 2);
//	if (c == '1') {
//		fonts.push_back(0);
//	}
//	else {
//		fonts.push_back(0);
//	}
//}
//
//void TextRenderInfo::addString(glm::vec2 offset, std::string string, glm::vec2 scale, int32_t depth) {
//	for (auto c : string) {
//		addChar(offset, c, scale, depth);
//		offset.x += scale.x;
//	}
//}
//
//void TextRenderInfo::addText(TextRenderer& textRenderer, CameraInfo& cameraInfo, glm::vec2 pos, float width, int32_t fontSize, std::string text, int32_t depth) {
//	glm::vec2 fontSize2 = { 2 * fontSize / textRenderer.fontWidth,  2 * fontSize };
//	glm::vec2 fontScale = fontSize2 / glm::vec2(cameraInfo.x, cameraInfo.y);
//
//	//clampView(fontScale);
//
//	//int viewSize = glm::floor((pos2.y - pos1.y) / fontScale.y);
//
//	glm::vec2 start = { pos.x, pos.y - fontScale.y };
//
//	addString(start, text, fontScale, depth);
//
//	//glm::vec2 offset = { pos1.x, pos2.y - fontScale.y };
//	//for (int32_t j = view[0]; j <= view[1]; j++) {
//	//	auto& line = lines[j];
//	//	offset[0] = pos1.x;
//	//	std::string prefix = "";
//	//	prefix += std::to_string(j) + "| ";
//
//	//	uiRenderInfo.textRenderInfo.addString(offset, prefix, fontScale);
//	//	for (std::size_t i = 0; i < line.size(); i++) {
//	//		if (line[i] == '\n') {
//	//			offset[1] -= fontScale.y;
//	//			offset[0] = pos1.x;
//	//		}
//	//		else {
//	//			uiRenderInfo.textRenderInfo.addChar(offset, line[i], fontScale);
//	//		}
//	//	}
//	//	offset[1] -= fontScale.y;
//	//	if (offset[1] + fontScale.y > pos2.y) break;
//	//}
//}
//
//void TextRenderInfo::addTexts(TextRenderer& textRenderer, CameraInfo& cameraInfo, glm::vec2 pos, float width, int32_t fontSize, std::vector<std::string>& texts, int32_t depth) {
//	glm::vec2 fontSize2 = { 2 * fontSize / textRenderer.fontWidth,  2 * fontSize };
//	glm::vec2 fontScale = fontSize2 / glm::vec2(cameraInfo.x, cameraInfo.y);
//
//	glm::vec2 start = pos;
//
//	for (auto& text : texts) {
//		addText(textRenderer, cameraInfo, start, width, fontSize, text, depth);
//		start.y -= fontScale.y;
//	}
//}
//

WindowTextRenderInfo::WindowTextRenderInfo(ScreenRectangle rect, Fonts::Font font_, bool lineWrap_) {
	this->font = font_;
	this->screenRectangle = rect;
	this->lineWrap = lineWrap_;
}

void WindowTextRenderInfo::addChar(char c) {
	FontInfo& fontInfo = Locator<Fonts>::ref().getFont(this->font);
	addCharInternal(c, fontInfo);
}

void WindowTextRenderInfo::addString(std::string text) {
	FontInfo& fontInfo = Locator<Fonts>::ref().getFont(this->font);
	for (char c : text) {
		addCharInternal(c, fontInfo);
	}
}

void WindowTextRenderInfo::newLine() {
	this->nextPos.x = -1.0f;
	this->nextPos.y -= this->laneWidth;
	this->laneWidth = 0.0f;
}

void WindowTextRenderInfo::addCharInternal(char c, FontInfo& fontInfo) {
	glm::vec2 size = glm::vec2(fontInfo.charSize[static_cast<int32_t>(c)]) / glm::vec2(this->screenRectangle.screenPixels) / this->screenRectangle.size() * 4.0f;
	if (c == '\n') {
		this->newLine();
	}
	else {
		if (this->lineWrap && this->nextPos.x + size.x > 1.0f) {
			this->newLine();
		}

		glm::vec2 addPos = this->nextPos;
		addPos.y -= size.y;

		this->laneWidth = glm::max(this->laneWidth, size.y);

		this->pos.push_back(glm::vec4(addPos, size));
		this->uvs.push_back(fontInfo.charUV[static_cast<int32_t>(c)]);

		this->nextPos.x += size.x;
	}
}
