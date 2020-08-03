#include "common.h"

#include "ScreenRectangle.h"

glm::vec2 ScreenRectangle::getTopLeft() {
	return glm::vec2(this->bot.x, this->top.y);
}

glm::vec2 ScreenRectangle::getBottomRight() {
	return glm::vec2(this->top.x, this->bot.y);
}

glm::vec2 ScreenRectangle::getTopRight() {
	return this->top;
}

glm::vec2 ScreenRectangle::getBottomLeft() {
	return this->bot;
}

float ScreenRectangle::getWidth() {
	return this->top.x - this->bot.x;
}

float ScreenRectangle::getHeight() {
	return this->top.y - this->bot.y;
}

glm::vec2 ScreenRectangle::size() {
	return glm::abs(this->bot - this->top);
}

glm::ivec2 ScreenRectangle::getPixelSize() {
	return this->screenPixels;
}

glm::ivec2 ScreenRectangle::getPixelPos() {
	// TODO
	assert(false);
	return glm::ivec2(0);
}

bool ScreenRectangle::contains(glm::vec2 p) {
	return (this->bot.x <= p.x) && (p.x <= this->top.x) && (this->bot.y <= p.y) && (p.y <= this->top.y);
}

void ScreenRectangle::scaleAnchorTopLeft(glm::vec2 s) {
	glm::vec2 v1 = getTopLeft();
	glm::vec2 v2 = getBottomRight();
	//s.y *= -1.0f;
	glm::vec2 diag = (v2 - v1) * s;

	set(v1, v1 + diag);
}

void ScreenRectangle::set(glm::vec2 p1, glm::vec2 p2) {
	this->bot = glm::min(p1, p2);
	this->top = glm::max(p1, p2);
}

void ScreenRectangle::setTopLeft(glm::vec2 p) {
	this->bot.x = p.x;
	this->top.y = p.y;
}

void ScreenRectangle::setBottomRight(glm::vec2 p) {
	this->bot.y = p.y;
	this->top.x = p.x;
}

void ScreenRectangle::setTopRight(glm::vec2 p) {
	this->top = p;
}

void ScreenRectangle::setBottomLeft(glm::vec2 p) {
	this->bot = p;
}

void ScreenRectangle::translate(glm::vec2 p) {
	this->bot += p;
	this->top += p;
}

void ScreenRectangle::setHeight(float h) {
	this->bot.y = this->top.y - h;
}

void ScreenRectangle::setWidth(float w) {
	this->top.x = this->bot.x + w;
}

bool ScreenRectangle::equals(ScreenRectangle& other) {
	return this->bot == other.bot && this->top == other.top && this->screenPixels == other.screenPixels;
}

