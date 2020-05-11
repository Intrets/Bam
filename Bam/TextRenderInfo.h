#pragma once

class TextRenderer;
struct CameraInfo;

struct TextRenderInfo
{
	std::vector<glm::vec3> offsets;
	std::vector<int> chars;
	std::vector<glm::vec2> scales;

	void addChar(glm::vec2 offset, char c, glm::vec2 scale);
	void addString(glm::vec2 offset, std::string string, glm::vec2 scale);
	void addText(TextRenderer &textRenderer, CameraInfo& cameraInfo, glm::vec2 pos, float width, int fontSize, std::string text);
};
