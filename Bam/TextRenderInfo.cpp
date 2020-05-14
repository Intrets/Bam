#include "common.h"

#include "TextRenderInfo.h"
#include "TextRenderer.h"
#include "CameraInfo.h"
#include "StringHelpers.h"

void TextRenderInfo::addChar(glm::vec2 offset, char c, glm::vec2 scale) {
	chars.push_back(c);
	offsets.push_back(glm::vec3(offset, 0));
	scales.push_back(scale);
}

void TextRenderInfo::addString(glm::vec2 offset, std::string string, glm::vec2 scale) {
	for (auto c : string) {
		addChar(offset, c, scale);
		offset.x += scale.x;
	}
}

void TextRenderInfo::addText(TextRenderer& textRenderer, CameraInfo& cameraInfo, glm::vec2 pos, float width, int32_t fontSize, std::string text) {
	glm::vec2 fontSize2 = { 2 * fontSize / textRenderer.fontWidth,  2 * fontSize };
	glm::vec2 fontScale = fontSize2 / glm::vec2(cameraInfo.x, cameraInfo.y);

	//clampView(fontScale);

	//int viewSize = glm::floor((pos2.y - pos1.y) / fontScale.y);

	glm::vec2 start = { pos.x, pos.y - fontScale.y };

	addString(start, text, fontScale);

	//glm::vec2 offset = { pos1.x, pos2.y - fontScale.y };
	//for (int32_t j = view[0]; j <= view[1]; j++) {
	//	auto& line = lines[j];
	//	offset[0] = pos1.x;
	//	std::string prefix = "";
	//	prefix += std::to_string(j) + "| ";

	//	uiRenderInfo.textRenderInfo.addString(offset, prefix, fontScale);
	//	for (std::size_t i = 0; i < line.size(); i++) {
	//		if (line[i] == '\n') {
	//			offset[1] -= fontScale.y;
	//			offset[0] = pos1.x;
	//		}
	//		else {
	//			uiRenderInfo.textRenderInfo.addChar(offset, line[i], fontScale);
	//		}
	//	}
	//	offset[1] -= fontScale.y;
	//	if (offset[1] + fontScale.y > pos2.y) break;
	//}
}

void TextRenderInfo::addTexts(TextRenderer& textRenderer, CameraInfo& cameraInfo, glm::vec2 pos, float width, int32_t fontSize, std::vector<std::string>& texts) {
	glm::vec2 fontSize2 = { 2 * fontSize / textRenderer.fontWidth,  2 * fontSize };
	glm::vec2 fontScale = fontSize2 / glm::vec2(cameraInfo.x, cameraInfo.y);

	glm::vec2 start = { pos.x, pos.y - fontScale.y };

	for (auto& text : texts) {
		addText(textRenderer, cameraInfo, start, width, fontSize, text);
		start.y -= fontScale.y;
	}
}

