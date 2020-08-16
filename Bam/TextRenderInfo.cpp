#include "common.h"

#include "TextRenderInfo.h"
#include "TextRenderer.h"
#include "CameraInfo.h"
#include "StringHelpers.h"
#include "RenderInfo.h"
#include "Rectangle.h"
#include "Colors.h"

WindowTextRenderInfo::WindowTextRenderInfo(ScreenRectangle rect, bool lineWrap_) {
	this->screenRectangle = rect;
	this->lineWrap = lineWrap_;
}

void WindowTextRenderInfo::addString(Fonts::Font font, std::string text) {
	FontInfo& fontInfo = Locator<Fonts>::ref().getFont(font);

	for (char c : text) {
		glm::vec2 size = glm::vec2(fontInfo.charSize[static_cast<int32_t>(c)]) / glm::vec2(this->screenRectangle.screenPixels) / this->screenRectangle.size() * 4.0f;

		glm::vec2 addPos;

		if (this->lineWrap && this->nextPos.x + size.x > 1.0f) {
			this->newLine();
		}

		addPos = this->nextPos;
		addPos.y -= size.y;

		this->laneHeight = glm::max(this->laneHeight, size.y);

		glm::vec2 vertRange = glm::vec2(addPos.y, addPos.y + size.y);
		glm::vec2 horRange = glm::vec2(addPos.x, addPos.x + size.x);

		auto it = this->lines.find(vertRange);
		if (it == this->lines.end()) {
			it = this->lines.insert({ vertRange, {} }).first;
		}
		it->second[horRange] = static_cast<int32_t>(uvs.size());


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
	return this->screenRectangle.size() * this->renderedSize / 2.0f;
}

std::optional<int32_t> WindowTextRenderInfo::getIndex(glm::vec2 p) {
	auto line = this->lines.find(glm::vec2(p.y));
	if (line == this->lines.end()) {
		return std::nullopt;
	}
	auto index = line->second.find(glm::vec2(p.x));
	if (index == line->second.end()) {
		return std::nullopt;
	}
	return index->second;
}

std::optional<glm::vec4> WindowTextRenderInfo::getCursorPos(int32_t index) {
	if (indexInVector(index, this->pos)) {
		return this->pos[index];
	}
	return std::nullopt;
}

void Text::invalidateCache() {
	this->cachedRenderInfo = std::nullopt;
}

void Text::makeRenderInfo(ScreenRectangle screenRectangle, Fonts::Font font, bool wrap) {
	this->lastScreenRectangle = screenRectangle;
	this->lastFont = font;
	this->lastWrap = wrap;
	WindowTextRenderInfo textInfo(screenRectangle, wrap);

	for (auto& line : lines) {
		textInfo.addString(font, line);
	}

	auto maybeCursorQuad = textInfo.getCursorPos(this->cursorIndex);
	if (maybeCursorQuad.has_value()) {
		glm::vec4& cursorQuad = maybeCursorQuad.value();
		Rectangle cursorRect;
		cursorRect.bot = glm::vec2(cursorQuad[0], cursorQuad[1]);
		cursorRect.top = cursorRect.bot + glm::vec2(cursorQuad[2], cursorQuad[3]);

		Rectangle viewRect;
		viewRect.bot = glm::vec2(-1.0f) + this->view;
		viewRect.top = glm::vec2(1.0f) + this->view;

		float leftDist = cursorRect.getLeft() - viewRect.getLeft();
		float rightDist = viewRect.getRight() - cursorRect.getRight();

		float botDist = cursorRect.getBot() - viewRect.getBot();
		float topDist = viewRect.getTop() - cursorRect.getTop();

		if (leftDist < 0.0f) {
			this->view.x += leftDist;
		}
		else if (rightDist < 0.0f) {
			this->view.x -= rightDist;
		}

		if (botDist < 0.0f) {
			this->view.y += botDist;
		}
		else if (topDist < 0.0f) {
			this->view.y -= topDist;
		}
	}

	textInfo.offset = -this->view;

	this->cachedRenderInfo = textInfo;
}

int32_t Text::addRenderInfo(ScreenRectangle screenRectangle, RenderInfo& renderInfo, Fonts::Font font, int32_t depth, bool wrap, int32_t tick, bool renderCursor) {
	if (!cachedRenderInfo.has_value()) {
		this->makeRenderInfo(screenRectangle, font, wrap);
	}
	auto& textRenderInfo = cachedRenderInfo.value();

	textRenderInfo.setDepth(depth++);
	renderInfo.textRenderInfo.windowTextRenderInfos.push_back(textRenderInfo);

	if (indexInVector(this->cursorIndex, textRenderInfo.pos)) {
		glm::vec4 v = textRenderInfo.pos[this->cursorIndex];
		glm::vec2 a = (glm::vec2(v[0], v[1]) - this->view) / 2.0f + 0.5f;
		glm::vec2 b = glm::vec2(v[2], v[3]) / 2.0f;
		a *= this->lastScreenRectangle.size();
		b *= this->lastScreenRectangle.size();

		a += this->lastScreenRectangle.getBottomLeft();
		//glm::vec2 db = glm::vec2(b.x, b.y / 10);

		if (renderCursor && tick % 60 < 30) {
			renderInfo.uiRenderInfo.addRectangle(a, a + b, COLORS::CURSOR, depth++);
		}
	}


	return depth;
}

bool Text::deleteChar() {
	if (this->cursor.y >= this->lines.size() - 1) {
		return false;
	}
	if (this->cursor.y == this->lines.size() - 2 && this->cursor.x == this->lines[this->cursor.y].size() - 1) {
		return false;
	}
	if (this->cursor.x == this->lines[this->cursor.y].size() - 1) {
		this->lines[this->cursor.y].erase(this->lines[this->cursor.y].end() - 1);
		this->lines[this->cursor.y] += this->lines[this->cursor.y + 1];
		this->lines.erase(this->lines.begin() + this->cursor.y + 1, this->lines.begin() + this->cursor.y + 2);
	}
	else {
		this->lines[this->cursor.y].erase(this->cursor.x, 1);
		this->cursor.x = glm::max(this->cursor.x, 0);
	}
	this->invalidateCache();
	return true;
}

bool Text::backspaceChar() {
	if (this->cursor == glm::ivec2(0)) {
		return false;
	}
	if (this->cursor.x == 0) {
		this->cursor.x = static_cast<int32_t>(this->lines[this->cursor.y - 1].size());
		this->lines[this->cursor.y - 1].erase(this->lines[this->cursor.y - 1].end() - 1);
		this->lines[this->cursor.y - 1] += this->lines[this->cursor.y];
		this->lines.erase(this->lines.begin() + this->cursor.y, this->lines.begin() + this->cursor.y + 1);
		this->cursor.y--;
	}
	else {
		this->lines[this->cursor.y].erase(this->cursor.x - 1, 1);
		this->cursor.x = glm::max(this->cursor.x - 1, 0);
		this->cursorIndex = glm::max(this->cursorIndex - 1, 0);
	}
	this->invalidateCache();
	return true;
}

void Text::insertString(std::string text) {
	for (auto c : text) {
		if (c == '\n') {
			auto first = std::string(this->lines[this->cursor.y].begin(), this->lines[this->cursor.y].begin() + this->cursor.x);
			auto second = std::string(this->lines[this->cursor.y].begin() + this->cursor.x, this->lines[this->cursor.y].end());

			this->lines[this->cursor.y] = first + "\n";
			this->cursor.x = 0;
			this->cursor.y++;
			this->cursorIndex++;
			this->lines.insert(this->lines.begin() + this->cursor.y, second);
		}
		else {
			this->lines[this->cursor.y].insert(this->cursor.x, std::string(1, c));
			this->cursor.x++;
			this->cursorIndex++;
		}
	}
	this->invalidateCache();
}

void Text::moveCursor(glm::ivec2 p) {
	glm::ivec2 previous = this->cursor;
	this->cursorIndex -= previous.x;

	this->cursor += p;
	this->cursor = glm::max(this->cursor, glm::ivec2(0));

	if (this->lines.size() == 1) {
		this->cursor.y = 0;
	}
	else {
		this->cursor.y = glm::min(this->cursor.y, static_cast<int32_t>(this->lines.size() - 2));
	}
	this->cursor.x = glm::min(this->cursor.x, static_cast<int32_t>(this->lines[this->cursor.y].size()) - 1);

	if (this->cursor.y < previous.y) {
		for (int32_t line = this->cursor.y; line < previous.y; line++) {
			this->cursorIndex -= static_cast<int32_t>(this->lines[line].size());
		}
	}
	else {
		for (int32_t line = previous.y; line < this->cursor.y; line++) {
			this->cursorIndex += static_cast<int32_t>(this->lines[line].size());
		}
	}
	this->cursorIndex += this->cursor.x;

	if (!this->cachedRenderInfo.has_value()) {
		this->makeRenderInfo(this->lastScreenRectangle, this->lastFont, this->lastWrap);
	}

	auto maybeCursorQuad = this->cachedRenderInfo.value().getCursorPos(this->cursorIndex);
	if (maybeCursorQuad.has_value()) {
		glm::vec4& cursorQuad = maybeCursorQuad.value();
		Rectangle cursorRect;
		cursorRect.bot = glm::vec2(cursorQuad[0], cursorQuad[1]);
		cursorRect.top = cursorRect.bot + glm::vec2(cursorQuad[2], cursorQuad[3]);

		Rectangle viewRect;
		viewRect.bot = glm::vec2(-1.0f) + this->view;
		viewRect.top = glm::vec2(1.0f) + this->view;

		float leftDist = cursorRect.getLeft() - viewRect.getLeft();
		float rightDist = viewRect.getRight() - cursorRect.getRight();

		float botDist = cursorRect.getBot() - viewRect.getBot();
		float topDist = viewRect.getTop() - cursorRect.getTop();

		if (leftDist < 0.0f) {
			this->view.x += leftDist;
		}
		else if (rightDist < 0.0f) {
			this->view.x -= rightDist;
		}

		if (botDist < 0.0f) {
			this->view.y += botDist;
		}
		else if (topDist < 0.0f) {
			this->view.y -= topDist;
		}
	}

	this->cachedRenderInfo.value().offset = -this->view;
}

void Text::selectIndex(int32_t index) {
	int32_t lineIndex = 0;
	int32_t i = index;

	for (auto& line : this->lines) {
		if (i - static_cast<int32_t>(line.size()) < 0) {
			this->cursor = glm::ivec2(i, lineIndex);
			this->cursorIndex = index;
			return;
		}
		i -= static_cast<int32_t>(line.size());
		lineIndex++;
	}
}

void Text::addLine(std::string text) {
	this->lines.back().append(text + "\n");
	this->lines.push_back("");
	this->invalidateCache();
}

void Text::addString(std::string text) {
	this->lines.back().append(text);
	this->invalidateCache();
}

void Text::setString(std::string text) {
	this->lines = std::vector<std::string>{ text };
	this->invalidateCache();
}

void Text::setLines(std::vector<std::string> lines_) {
	lines = {};
	for (auto& line : lines_) {
		lines.push_back(line + "\n");
	}
	this->invalidateCache();
}
