#pragma once

#include "Rectangle.h"
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

	// layering for UI rendering
	float depth;

	// view offset for render info
	glm::vec2 offset;

	// total size of entire text in [-1, 1] in screenRectangle space 
	// value in [0, inf)
	glm::vec2 renderedSize;

	// in [-1, 1] in screenRectangle space
	std::vector<glm::vec4> pos;

	// UV info for atlas texture
	std::vector<glm::vec4> uvs;

	// next position for character in screenRectangle space
	glm::vec2 nextPos = { -1.0f, 1.0f };

	// current line width
	float laneHeight = 0.0f;

	// render with linewrap
	bool lineWrap;

	struct cmp
	{
		bool operator()(const glm::vec2& a, const glm::vec2& b) const {
			return (a[0] < b[0]) && (a[1] < b[1]);
		}
	};

	// click to select characters support
	bool clickSelect;

	// lines for click cursor selecting
	std::map<glm::vec2, std::map<glm::vec2, int32_t, cmp>, cmp> lines;

	WindowTextRenderInfo(ScreenRectangle rect, bool lineWrap = false, bool clickSupport_ = false);

	void addString(Fonts::Font font, std::string text);
	void newLine();
	void setDepth(int32_t layer);
	void setDepth(float depth_);

	glm::vec2 getRenderedScreenSize();

	std::optional<int32_t> getIndex(glm::vec2 p);

	// first 2 Coordinates in [-1, 1] this->screenRectangle space. 
	// second 2 size in same space.
	std::optional<glm::vec4> getCursorPos(int32_t index) const;
};

struct TextRenderInfo
{
	std::vector<WindowTextRenderInfo> windowTextRenderInfos;
};

class Text
{
private:
	std::vector<std::string> lines = { "" };
	glm::ivec2 cursor;
	int32_t cursorIndex;

private:
	ScreenRectangle lastScreenRectangle;
	bool lastClickSupport;

	friend class UIOTextDisplay;

public:
	std::optional<WindowTextRenderInfo> cachedRenderInfo;
	Fonts::Font lastFont;
	bool lastWrap;

	// TODO: make view behave less weird when resizing window
	glm::vec2 view = { 0.0f, 0.0f };

	std::vector<std::string> const& getLines() const;
	std::vector<std::string>& getLinesMutable();
	std::optional<Rectangle> getCursorQuadScreen() const;

	void invalidateCache();
	void makeRenderInfo(ScreenRectangle screenRectangle, Fonts::Font font, bool wrap, bool clickSupport);

	int32_t addRenderInfo(ScreenRectangle screenRectangle, RenderInfo& renderInfo, Fonts::Font font, int32_t depth, bool wrap, int32_t tick, bool renderCursor, bool clickSupport);

	bool deleteChar();
	bool backspaceChar();

	void insertString(std::string text);

	void hideCursor();
	void moveCursor(glm::ivec2 p);
	void setCursor(glm::ivec2 p);
	void moveView(glm::ivec2 p);

	// (!) leaves in invalid state (!) need to move the cursor after using this
	void zeroCursor();

	glm::ivec2 getCursor();

	void selectIndex(int32_t index);

	// (!) will be left in invalid state if no lines are added (!)
	void empty();

	void addLine(std::string text);

	void setString(std::string text);
	void setLines(std::vector<std::string> lines);
};
