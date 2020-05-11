#include "common.h"

#include "UIRenderInfo.h"

void UIRenderInfo::addRectangle(glm::vec2 pos1, glm::vec2 pos2, glm::vec4 color) {
	scales.push_back(pos2 - pos1);
	positions.push_back(glm::vec3(pos1, 0));
	colors.push_back(color);
}
