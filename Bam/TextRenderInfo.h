#pragma once

class TextRenderer;
struct CameraInfo;

struct TextRenderInfo
{
	std::vector<glm::vec3> offsets;
	std::vector<int32_t> chars;
	std::vector<glm::vec2> scales;
	TextRenderer* textRendererRef;

	void addChar(glm::vec2 offset, char c, glm::vec2 scale);
	void addString(glm::vec2 offset, std::string string, glm::vec2 scale);
	void addText(TextRenderer &textRenderer, CameraInfo& cameraInfo, glm::vec2 pos, float width, int32_t fontSize, std::string text);
	void addTexts(TextRenderer &textRenderer, CameraInfo& cameraInfo, glm::vec2 pos, float width, int32_t fontSize, std::vector<std::string>& texts);
};
