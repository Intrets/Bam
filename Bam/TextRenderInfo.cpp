#include "common.h"

#include "TextRenderInfo.h"
#include "TextRenderer.h"
#include "CameraInfo.h"
#include "StringHelpers.h"
#include "RenderInfo.h"
#include "Rectangle.h"
#include "Colors.h"

WindowTextRenderInfo::WindowTextRenderInfo(ScreenRectangle rect, bool lineWrap_, bool clickSupport_) :
	screenRectangle(rect),
	lineWrap(lineWrap_),
	clickSelect(clickSupport_) {
}

void WindowTextRenderInfo::addString(FONTS::FONT font, std::string text) {
	FontInfo& fontInfo = Locator<Fonts>::ref().getFont(font);

	glm::ivec2 sizei = glm::floor(glm::vec2(this->screenRectangle.getPixelSize()) * this->screenRectangle.getAbsSize() / 2.0f);

	for (char c : text) {
		glm::vec2 size = glm::vec2(fontInfo.charSize[static_cast<int32_t>(c)]) / glm::vec2(sizei) * 2.0f;

		glm::vec2 addPos;

		if (this->lineWrap && this->nextPos.x + size.x > 1.0f && c != '\n') {
			this->newLine();
		}

		addPos = this->nextPos;
		addPos.y -= size.y;

		this->laneHeight = glm::max(this->laneHeight, size.y);

		if (this->clickSelect) {
			glm::vec2 vertRange = glm::vec2(addPos.y, addPos.y + size.y);
			glm::vec2 horRange = glm::vec2(addPos.x, addPos.x + size.x);

			auto it = this->lines.find(vertRange);
			if (it == this->lines.end()) {
				it = this->lines.insert({ vertRange, {} }).first;
			}
			it->second[horRange] = static_cast<int32_t>(uvs.size());
		}

		this->pos.push_back(glm::vec4(addPos, size));
		this->uvs.push_back(fontInfo.charUV[static_cast<int32_t>(c)]);

		this->nextPos.x += size.x;

		if (c == '\n') {
			this->newLine();
		}

		glm::vec2 addBot = addPos;
		addBot.x += size.x;
		glm::vec2 curSize = glm::abs(addBot - glm::vec2(-1.0f, 1.0f));
		this->renderedSize = glm::max(this->renderedSize, curSize);
	}
}

void WindowTextRenderInfo::newLine() {
	this->nextPos.x = -1.0f;
	this->nextPos.y -= this->laneHeight;
}

void WindowTextRenderInfo::setDepth(int32_t layer) {
	this->depth = 1 - 1 / static_cast<float>(1 + layer);
}

void WindowTextRenderInfo::setDepth(float depth_) {
	this->depth = depth_;
}

glm::vec2 WindowTextRenderInfo::getRenderedScreenSize() {
	return this->screenRectangle.getAbsSize() * this->renderedSize / 2.0f;
}

std::optional<int32_t> WindowTextRenderInfo::getIndex(glm::vec2 p) {
	auto line = this->lines.find(glm::vec2(p.y));
	if (line == this->lines.end()) {
		return std::nullopt;
	}
	auto index = line->second.find(glm::vec2(p.x));
	if (index == line->second.end()) {
		if (line->second.size() == 0) {
			return std::nullopt;
		}
		else {
			auto t = line->second.end();
			return (--t)->second;
		}
	}
	return index->second;
}

std::optional<glm::vec4> WindowTextRenderInfo::getCursorPos(int32_t index) const {
	if (indexInVector(index, this->pos)) {
		return this->pos[index];
	}
	return std::nullopt;
}

void Text::validateView() {
	if (!this->cachedRenderInfo.has_value()) {
		this->makeRenderInfo(this->lastScreenRectangle, this->lastFont, this->lastWrap, this->lastClickSupport);
	}

	if (auto const& maybeCursorQuad = this->cachedRenderInfo.value().getCursorPos(cursorIndex)) {
		auto& view = this->viewCache.getVal();

		glm::vec4 const& cursorQuad = maybeCursorQuad.value();
		Rectangle cursorRect;
		cursorRect.setBottomLeft(glm::vec2(cursorQuad[0], cursorQuad[1]));
		cursorRect.setTopRight(cursorRect.getBottomLeft() + glm::vec2(cursorQuad[2], cursorQuad[3]));

		Rectangle viewRect;
		viewRect.setBottomLeft(glm::vec2(-1.0f) + view);
		viewRect.setTopRight(glm::vec2(1.0f) + view);

		float leftDist = cursorRect.getLeft() - viewRect.getLeft();
		float rightDist = viewRect.getRight() - cursorRect.getRight();

		float botDist = cursorRect.getBot() - viewRect.getBot();
		float topDist = viewRect.getTop() - cursorRect.getTop();

		if (leftDist < 0.0f) {
			view.x += leftDist;
		}
		else if (rightDist < 0.0f) {
			view.x -= rightDist;
		}

		if (botDist < 0.0f) {
			view.y += botDist;
		}
		else if (topDist < 0.0f) {
			view.y -= topDist;
		}
	}

	this->viewCache.validate();
}

glm::vec2 Text::getView() {
	if (!this->viewCache.isValid()) {
		this->validateView();
	}
	return this->viewCache.getVal();
}

std::vector<std::string> const& Text::getLines() const {
	return this->lines;
}

std::vector<std::string>& Text::getLinesMutable() {
	return this->lines;
}

std::optional<Rectangle> Text::getCursorQuadScreen() {
	if (!this->cachedRenderInfo.has_value()) {
		return std::nullopt;
	}
	auto const& maybeCursorShape = this->cachedRenderInfo.value().getCursorPos(this->cursorIndex);
	if (!maybeCursorShape.has_value()) {
		return std::nullopt;
	}
	auto const& cursorShape = maybeCursorShape.value();

	glm::vec2 a = (glm::vec2(cursorShape[0], cursorShape[1]) - this->getView()) / 2.0f + 0.5f;
	glm::vec2 b = glm::vec2(cursorShape[2], cursorShape[3]) / 2.0f;

	a *= this->lastScreenRectangle.getAbsSize();
	b *= this->lastScreenRectangle.getAbsSize();

	a += this->lastScreenRectangle.getBottomLeft();
	b += a;

	return Rectangle{ a, b };
}

void Text::invalidateCache() {
	this->cachedRenderInfo = std::nullopt;
	this->viewCache.invalidate();
	this->cursorCache.invalidate();
}

void Text::makeRenderInfo(ScreenRectangle screenRectangle, FONTS::FONT font, bool wrap, bool clickSupport) {
	this->lastScreenRectangle = screenRectangle;

	this->lastFont = font;
	this->lastWrap = wrap;
	this->lastClickSupport = clickSupport;
	WindowTextRenderInfo textInfo(screenRectangle, wrap, clickSupport);

	for (auto& line : lines) {
		textInfo.addString(font, line);
	}

	this->lastRenderedSize = textInfo.renderedSize;
	this->cachedRenderInfo = std::move(textInfo);
}

int32_t Text::addRenderInfo(ScreenRectangle screenRectangle, RenderInfo& renderInfo, FONTS::FONT font, int32_t depth, bool wrap, int32_t tick, bool renderCursor, bool clickSupport) {
	if (!this->cachedRenderInfo.has_value()) {
		this->makeRenderInfo(screenRectangle, font, wrap, clickSupport);
	}

	auto& textRenderInfo = this->cachedRenderInfo.value();

	textRenderInfo.offset = -this->getView();
	textRenderInfo.setDepth(depth++);
	renderInfo.textRenderInfo.windowTextRenderInfos.push_back(textRenderInfo);

	if (renderCursor && periodic(tick, 30, 30)) {
		if (auto const& maybeQuad = this->getCursorQuadScreen()) {
			auto quad = maybeQuad.value();
			glm::vec2 bottomLeft = glm::max(screenRectangle.getBottomLeft(), glm::min(screenRectangle.getTopRight(), quad.getBottomLeft()));
			glm::vec2 topRight = glm::max(screenRectangle.getBottomLeft(), glm::min(screenRectangle.getTopRight(), quad.getTopRight()));
			renderInfo.uiRenderInfo.addRectangle(bottomLeft, topRight, COLORS::UI::CURSOR, depth++);
		}
	}

	return depth;
}

bool Text::deleteChar() {
	auto& cursor = this->cursorCache.getVal();

	if (cursor.y >= this->lines.size() - 1) {
		return false;
	}
	if (cursor.y == this->lines.size() - 2 && cursor.x == this->lines[cursor.y].size() - 1) {
		return false;
	}
	if (cursor.x == this->lines[cursor.y].size() - 1) {
		this->lines[cursor.y].erase(this->lines[cursor.y].end() - 1);
		this->lines[cursor.y] += this->lines[cursor.y + 1];
		this->lines.erase(this->lines.begin() + cursor.y + 1, this->lines.begin() + cursor.y + 2);
	}
	else {
		this->lines[cursor.y].erase(cursor.x, 1);
		cursor.x = glm::max(cursor.x, 0);
		this->cursorCache.invalidate();
	}

	this->invalidateCache();
	return true;
}

bool Text::backspaceChar() {
	auto& cursor = this->cursorCache.getVal();

	if (cursor == glm::ivec2(0)) {
		return false;
	}
	if (cursor.x == 0) {
		cursor.x = static_cast<int32_t>(this->lines[cursor.y - 1].size() - 1);
		cursor.x = glm::max(0, cursor.x);
		this->lines[cursor.y - 1].erase(this->lines[cursor.y - 1].end() - 1);
		this->lines[cursor.y - 1] += this->lines[cursor.y];
		this->lines.erase(this->lines.begin() + cursor.y, this->lines.begin() + cursor.y + 1);
		cursor.y--;
	}
	else {
		this->lines[cursor.y].erase(cursor.x - 1, 1);
		cursor.x = glm::max(cursor.x - 1, 0);
		cursorIndex = glm::max(cursorIndex - 1, 0);
	}

	this->invalidateCache();
	return true;
}

void Text::insertString(std::string text) {
	auto& cursor = this->cursorCache.getVal();

	for (auto c : text) {
		if (c == '\n') {
			auto first = std::string(this->lines[cursor.y].begin(), this->lines[cursor.y].begin() + cursor.x);
			auto second = std::string(this->lines[cursor.y].begin() + cursor.x, this->lines[cursor.y].end());

			this->lines[cursor.y] = first + "\n";
			cursor.x = 0;
			cursor.y++;
			cursorIndex++;
			this->lines.insert(this->lines.begin() + cursor.y, second);
		}
		else {
			this->lines[cursor.y].insert(cursor.x, std::string(1, c));
			cursor.x++;
			cursorIndex++;
		}
	}

	this->invalidateCache();
}

void Text::hideCursor() {
	this->cursorIndex = -1;
}

void Text::moveCursor(glm::ivec2 p) {
	auto cursor = this->cursorCache.getVal();

	cursor += p;
	cursor = glm::max(cursor, glm::ivec2(0));

	if (this->lines.size() == 1) {
		cursor.y = 0;
	}
	else {
		cursor.y = glm::clamp(cursor.y, 0, static_cast<int32_t>(this->lines.size()) - 2);
	}

	cursor.x = glm::clamp(cursor.x, 0, static_cast<int32_t>(this->lines[cursor.y].size() - 1));

	cursorIndex = 0;
	for (int32_t line = 0; line < cursor.y; line++) {
		cursorIndex += static_cast<int32_t>(this->lines[line].size());
	}
	cursorIndex += cursor.x;

	if (this->cursorCache.getVal() != cursor) {
		this->cursorCache = cursor;
		this->viewCache.invalidate();
	}
}

void Text::setCursor(glm::ivec2 p) {
	this->zeroCursor();
	this->moveCursor(p);
}

void Text::moveView(glm::ivec2 p) {
	int32_t pxHeight = Locator<Fonts>::ref().getFont(this->lastFont).charSize[0].y;

	if (this->lastScreenRectangle.getPixelSize().y == 0) {
		return;
	}

	float height = this->lastScreenRectangle.getHeight() / 2.0f * this->lastScreenRectangle.getPixelSize().y;

	float lineHeight = 2.0f * static_cast<float>(pxHeight) / height;

	auto& view = this->viewCache.getVal();

	view += lineHeight * glm::vec2(p) * 1.4f;

	// margin from top
	float topStop = this->lastRenderedSize.y;

	// margin from bottom
	float botStop = lineHeight;

	if (view.y < 0.0f) {
		if (topStop + view.y < lineHeight) {
			view.y = lineHeight - topStop;
		}
	}
	else {
		if (view.y > botStop - lineHeight) {
			view.y = botStop - lineHeight;
		}
	}
	this->viewCache.validate();
}

void Text::zeroCursor() {
	this->cursorCache = glm::ivec2(0, 0);
	this->viewCache.invalidate();
}

glm::ivec2 Text::getCursor() {
	return this->cursorCache.getVal();
}

void Text::selectIndex(int32_t index) {
	int32_t lineIndex = 0;
	int32_t i = index;

	for (auto& line : this->lines) {
		if (i - static_cast<int32_t>(line.size()) < 0) {
			this->cursorCache = glm::ivec2(i, lineIndex);
			this->cursorIndex = index;
			return;
		}
		i -= static_cast<int32_t>(line.size());
		lineIndex++;
	}
}

void Text::empty() {
	this->lines = { "" };
	this->invalidateCache();
}

void Text::addLine(std::string text) {
	this->lines.back().append(text + "\n");
	this->lines.push_back("");
	this->invalidateCache();
}

void Text::setString(std::string text) {
	this->empty();
	this->addLine(text);
}

void Text::setLines(std::vector<std::string> lines_) {
	this->lines = {};
	for (auto& line : lines_) {
		lines.push_back(line + "\n");
	}
	this->lines.push_back("");

	this->cursorCache = glm::ivec2(0, 0);
	this->cursorIndex = 0;
	this->invalidateCache();
}
