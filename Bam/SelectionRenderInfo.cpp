#include "common.h"
#include "SelectionRenderInfo.h"

void SelectionRenderInfo::addBox(glm::vec2 up_, glm::vec2 down_, glm::vec4 color) {
	glm::vec2 down = glm::min(up_, down_);
	glm::vec2 up = glm::max(up_, down_);

	this->offsets.push_back(up);
	glm::vec2 scale = glm::vec2(down - up);
	this->scales.push_back(scale);
	this->colors.push_back(color);
}
