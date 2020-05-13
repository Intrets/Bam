#pragma once

struct SelectionRenderInfo
{
	std::vector<glm::vec2> offsets;
	std::vector<glm::vec2> scales;
	std::vector<glm::vec4> colors;
	
	void addBox(glm::vec2 up, glm::vec2 down, glm::vec4 color = { 0.5, 0.5, 1, 0.5 });
};
