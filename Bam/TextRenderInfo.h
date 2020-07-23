#pragma once

#include "ScreenRectangle.h"
#include "Fonts.h"

class TextRenderer;
struct CameraInfo;

struct WindowTextRenderInfo
{
public:
	ScreenRectangle screenRectangle;
	float depth;

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
	void setDepth(int32_t layer);
	void setDepth(float depth_);

private:
	void addCharInternal(char c, FontInfo& fontInfo);
};

struct TextRenderInfo
{
	std::vector<WindowTextRenderInfo> windowTextRenderInfos;
};
