#pragma once

#include "ScreenRectangle.h"
#include "Fonts.h"
#include <optional>
#include <map>

class TextRenderer;
struct CameraInfo;
struct RenderInfo;

struct WindowTextRenderInfo
{
public:
	ScreenRectangle screenRectangle;
	float depth;

	glm::vec2 offset;

	std::vector<glm::vec4> pos;
	std::vector<glm::vec4> uvs;

	glm::vec2 nextPos = { -1.0f, 1.0f };
	float laneWidth = 0.0f;
	bool lineWrap;

	struct cmp
	{
		bool operator()(const glm::vec2& a, const glm::vec2& b) const {
			return (a[0] < b[0]) && (a[1] < b[1]);
		}
	};

	std::map<glm::vec2, std::map<glm::vec2, int32_t, cmp>, cmp> lines;

	WindowTextRenderInfo(ScreenRectangle rect, bool lineWrap = false);

	void addString(Fonts::Font font, std::string text);
	void newLine();
	void setDepth(int32_t layer);
	void setDepth(float depth_);

	std::optional<glm::vec4> getCursorPos(int32_t index);
};

struct TextRenderInfo
{
	std::vector<WindowTextRenderInfo> windowTextRenderInfos;
};

class Text
{
public:
	std::vector<std::string> lines = { "" };

	glm::ivec2 cursor;
	int32_t cursorIndex;

	std::optional<WindowTextRenderInfo> cachedRenderInfo;
	ScreenRectangle lastScreenRectangle;
	Fonts::Font lastFont;
	bool lastWrap;

	glm::vec2 view = { 0.0f, 0.0f };

	void invalidateCache();
	void makeRenderInfo(ScreenRectangle screenRectangle, Fonts::Font font, bool wrap);

	int32_t addRenderInfo(ScreenRectangle screenRectangle, RenderInfo& renderInfo, Fonts::Font font, int32_t depth, bool wrap);

	bool deleteChar();
	bool backspaceChar();
	void insertString(std::string text);
	void moveCursor(glm::ivec2 p);

	void addLine(std::string text);
};
