#pragma once

struct UIRenderInfo
{
	std::vector<glm::vec2> scales;
	std::vector<glm::vec3> positions;
	std::vector<glm::vec4> colors;

	void addRectangle(glm::vec2 pos1, glm::vec2 pos2, glm::vec4 color);
};

