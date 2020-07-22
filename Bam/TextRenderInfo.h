#pragma once

#include "ScreenRectangle.h"

#include "Fonts.h"

class TextRenderer;
struct CameraInfo;

struct WindowTextRenderInfo
{
public:
	ScreenRectangle screenRectangle;

	std::vector<glm::vec4> pos;
	std::vector<glm::vec4> uvs;

	Fonts::Font font;
	glm::vec2 nextPos = { -1.0f, 1.0f };
	float laneWidth = 0.0f;
	bool lineWrap;

	WindowTextRenderInfo(ScreenRectangle rect, Fonts::Font font_, bool lineWrap = false);

	void addChar(char c);
	void addString(std::string text);
	void newLine();

private:
	void addCharInternal(char c, FontInfo& fontInfo);
};

struct TextRenderInfo
{
	std::vector<WindowTextRenderInfo> windowTextRenderInfos;

	//void addChar(glm::vec2 offset, char c, glm::vec2 scale, int32_t depth = 0);
	//void addString(glm::vec2 offset, std::string string, glm::vec2 scale, int32_t depth = 0);
	//void addText(TextRenderer &textRenderer, CameraInfo& cameraInfo, glm::vec2 pos, float width, int32_t fontSize, std::string text, int32_t depth = 0);
	//void addTexts(TextRenderer &textRenderer, CameraInfo& cameraInfo, glm::vec2 pos, float width, int32_t fontSize, std::vector<std::string>& texts, int32_t depth = 0);
};
