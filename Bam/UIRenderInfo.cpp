#include "common.h"

#include "UIRenderInfo.h"

void UIRenderInfo::addRectangle(glm::vec2 pos1, glm::vec2 pos2, glm::vec4 color) {
	//glm::vec2 down = glm::min(pos1, pos2);
	//glm::vec2 up = glm::max(pos1, pos2);
	scales.push_back(pos2 - pos1);
	positions.push_back(glm::vec3(pos1, 0));
	colors.push_back(color);
}
