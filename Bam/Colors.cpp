#include "common.h"

#include "Colors.h"

glm::vec4 COLORS::DARKEN(glm::vec4 color) {
	return color * glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
}
