#pragma once

namespace COLORS
{
	static glm::vec4 CURSOR = glm::vec4(0.78f, 0.0f, 0.0f, 1.0f);
	static glm::vec4 RED = glm::vec4(1.0f, 0.6f, 0.6f, 1.0f);
	static glm::vec4 GREEN = glm::vec4(0.6f, 1.0f, 0.6f, 1.0f);

	glm::vec4 DARKEN(glm::vec4 color);

	static glm::vec4 BACKGROUND = glm::vec4(0.9f, 0.9f, 0.9f, 1.0f);
	static glm::vec4 FOREGROUND = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	static glm::vec4 FOCUSSED = glm::vec4(0.95f, 0.95f, 0.95f, 1.0f);
}
