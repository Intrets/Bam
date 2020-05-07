#include "common.h"
#include "SelectionRenderInfo.h"

void SelectionRenderInfo::addBox(glm::vec2 up_, glm::vec2 down_, glm::vec3 color) {
	glm::vec2 down = glm::min(up_, down_);
	glm::vec2 up = glm::max(up_, down_);

	down += glm::vec2(-0.1, -0.1);
	up += glm::vec2(1.1, 1.1);

	offsets.push_back(up);
	glm::vec2 scale = glm::vec2(down - up);
	scales.push_back(scale);
	colors.push_back(glm::vec4(color, 0.5));
}
