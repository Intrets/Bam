#include "common.h"

#include "ScreenRectangle.h"

glm::vec2 ScreenRectangle::getTopLeft() {
	return glm::vec2(bot.x, top.y);
}

glm::vec2 ScreenRectangle::getBottomRight() {
	return glm::vec2(top.x, bot.y);
}

glm::vec2 ScreenRectangle::getTopRight() {
	return top;
}

glm::vec2 ScreenRectangle::getBottomLeft() {
	return bot;
}

glm::vec2 ScreenRectangle::size() {
	return glm::abs(bot - top);
}

float ScreenRectangle::width() {
	return top.y - bot.y;
}

glm::ivec2 ScreenRectangle::getPixelSize() {
	return screenPixels;
}

glm::ivec2 ScreenRectangle::getPixelPos() {
	// TODO
	assert(false);
	return glm::ivec2(0);
}

bool ScreenRectangle::contains(glm::vec2 p) {
	return (bot.x < p.x) && (p.x < top.x) && (bot.y < p.y) && (p.y < top.y);
}

void ScreenRectangle::scaleAnchorTopLeft(glm::vec2 s) {
	glm::vec2 v1 = getTopLeft();
	glm::vec2 v2 = getBottomRight();
	//s.y *= -1.0f;
	glm::vec2 diag = (v2 - v1) * s;

	set(v1, v1 + diag);
}

void ScreenRectangle::set(glm::vec2 p1, glm::vec2 p2) {
	bot = glm::min(p1, p2);
	top = glm::max(p1, p2);
}

void ScreenRectangle::setTopLeft(glm::vec2 p) {
	bot.x = p.x;
	top.y = p.y;
}

void ScreenRectangle::setBottomRight(glm::vec2 p) {
	bot.y = p.y;
	top.x = p.x;
}

void ScreenRectangle::setTopRight(glm::vec2 p) {
	top = p;
}

void ScreenRectangle::setBottomLeft(glm::vec2 p) {
	bot = p;
}

void ScreenRectangle::translate(glm::vec2 p) {
	bot += p;
	top += p;
}

void ScreenRectangle::setHeight(float h) {
	bot.y = top.y - h;
}

void ScreenRectangle::setWidth(float w) {
	top.x = bot.x + w;
}

