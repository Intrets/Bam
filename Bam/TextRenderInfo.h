#pragma once

class TextRenderer;
struct CameraInfo;

struct TextRenderInfo
{
	std::vector<glm::vec3> offsets;
	std::vector<int32_t> chars;
	std::vector<glm::vec2> scales;
	TextRenderer* textRendererRef;

	void addChar(glm::vec2 offset, char c, glm::vec2 scale, int32_t depth = 0);
	void addString(glm::vec2 offset, std::string string, glm::vec2 scale, int32_t depth = 0);
	void addText(TextRenderer &textRenderer, CameraInfo& cameraInfo, glm::vec2 pos, float width, int32_t fontSize, std::string text, int32_t depth = 0);
	void addTexts(TextRenderer &textRenderer, CameraInfo& cameraInfo, glm::vec2 pos, float width, int32_t fontSize, std::vector<std::string>& texts, int32_t depth = 0);
};
