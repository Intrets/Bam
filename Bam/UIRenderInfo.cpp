#include "common.h"

#include "UIRenderInfo.h"

void UIRenderInfo::addRectangle(glm::vec2 pos1, glm::vec2 pos2, glm::vec4 color) {
	scales.push_back(pos2 - pos1);
	positions.push_back(glm::vec3(pos1, 0));
	colors.push_back(color);
	//glm::vec2 down = glm::min(pos1, pos2);
	//glm::vec2 up = glm::max(pos1, pos2);
	//addRectangle(pos1, pos2, color, -100);
}

void UIRenderInfo::addRectangle(glm::vec2 pos1, glm::vec2 pos2, glm::vec4 color, int32_t depth) {
	scales.push_back(pos2 - pos1);
	positions.push_back(glm::vec3(pos1, -static_cast<float>(depth) / 1000.0f));
	colors.push_back(color);
}
