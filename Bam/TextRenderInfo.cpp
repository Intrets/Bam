#include "common.h"

#include "TextRenderInfo.h"
#include "TextRenderer.h"
#include "CameraInfo.h"
#include "StringHelpers.h"

WindowTextRenderInfo::WindowTextRenderInfo(ScreenRectangle rect, Fonts::Font font_, bool lineWrap_) {
	this->font = font_;
	this->screenRectangle = rect;
	this->lineWrap = lineWrap_;
}

void WindowTextRenderInfo::addChar(char c) {
	FontInfo& fontInfo = Locator<Fonts>::ref().getFont(this->font);
	addCharInternal(c, fontInfo);
}

void WindowTextRenderInfo::addString(std::string text) {
	FontInfo& fontInfo = Locator<Fonts>::ref().getFont(this->font);
	for (char c : text) {
		addCharInternal(c, fontInfo);
	}
}

void WindowTextRenderInfo::newLine() {
	this->nextPos.x = -1.0f;
	this->nextPos.y -= this->laneWidth;
}

void WindowTextRenderInfo::setDepth(int32_t layer) {
	this->depth = 1 - 1 / static_cast<float>(1 + layer);
}

void WindowTextRenderInfo::setDepth(float depth_) {
	this->depth = depth_;
}

void WindowTextRenderInfo::addCharInternal(char c, FontInfo& fontInfo) {
	glm::vec2 size = glm::vec2(fontInfo.charSize[static_cast<int32_t>(c)]) / glm::vec2(this->screenRectangle.screenPixels) / this->screenRectangle.size() * 4.0f;
	if (c == '\n') {
		this->newLine();
	}
	else {
		if (this->lineWrap && this->nextPos.x + size.x > 1.0f) {
			this->newLine();
		}

		glm::vec2 addPos = this->nextPos;
		addPos.y -= size.y;

		this->laneWidth = glm::max(this->laneWidth, size.y);

		this->pos.push_back(glm::vec4(addPos, size));
		this->uvs.push_back(fontInfo.charUV[static_cast<int32_t>(c)]);

		this->nextPos.x += size.x;
	}
}
