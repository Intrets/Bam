#include "common.h"

#include "ScreenRectangle.h"

glm::vec2 ScreenRectangle::topLeft() {
	return glm::vec2(bot.x, top.y);
}

glm::vec2 ScreenRectangle::bottomRight() {
	return glm::vec2(top.x, bot.y);
}

glm::ivec2 ScreenRectangle::getPixelSize() {
	// TODO
	assert(false);
	return glm::ivec2(0);
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
	glm::vec2 v1 = topLeft();
	glm::vec2 v2 = bottomRight();
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

