#include "common.h"

#include <GLM/glm.hpp>
#include <GLM/gtc/constants.hpp>

#include "Rectangle.h"

Rectangle::Rectangle(glm::vec2 b, glm::vec2 t) : bot(b), top(t) {
}

Rectangle::Rectangle(float botx, float boty, float topx, float topy) : bot(botx, boty), top(topx, topy) {
}

void Rectangle::set(glm::vec2 p1, glm::vec2 p2) {
	this->bot = glm::min(p1, p2);
	this->top = glm::max(p1, p2);
}

void Rectangle::set(ScreenRectangle rec) {
	this->bot = rec.bot;
	this->top = rec.top;
}

void Rectangle::set(Rectangle rec) {
	this->bot = rec.bot;
	this->top = rec.top;
}

bool Rectangle::equals(Rectangle& other) const {
	return norm2(this->bot - other.bot) < glm::epsilon<float>() && norm2(this->top - other.top) < glm::epsilon<float>();
}

bool Rectangle::equals(ScreenRectangle& other) const {
	return norm2(this->bot - other.bot) < glm::epsilon<float>() && norm2(this->top - other.top) < glm::epsilon<float>();
}

bool Rectangle::contains(glm::vec2 p) const {
	return (this->bot.x < p.x) && (p.x < this->top.x) && (this->bot.y < p.y) && (p.y < this->top.y);
}

bool Rectangle::contains(Rectangle& rec) const {
	return this->contains(rec.bot) && this->contains(rec.top);
}

bool Rectangle::contains(ScreenRectangle& rec) const {
	return this->contains(rec.bot) && this->contains(rec.top);
}

float Rectangle::getLeft() const {
	return bot.x;
}

float Rectangle::getRight() const {
	return top.x;
}

float Rectangle::getTop() const {
	return top.y;
}

float Rectangle::getBot() const {
	return bot.y;
}

glm::vec2 Rectangle::getAbsSize() const {
	return glm::abs(this->bot - this->top);
}

glm::vec2 Rectangle::getTopLeft() const {
	return glm::vec2(this->bot.x, this->top.y);
}

glm::vec2 Rectangle::getBottomRight() const {
	return glm::vec2(this->top.x, this->bot.y);
}

glm::vec2 Rectangle::getTopRight() const {
	return this->top;
}

glm::vec2 Rectangle::getBottomLeft() const {
	return this->bot;
}

void Rectangle::setTop(float s) {
	this->top.y = s;
}

void Rectangle::setBot(float s) {
	this->bot.y = s;
}

void Rectangle::setRight(float s) {
	this->top.x = s;
}

void Rectangle::setLeft(float s) {
	this->bot.x = s;
}

void Rectangle::translate(glm::vec2 p) {
	this->bot += p;
	this->top += p;
}

void Rectangle::translateLeft(float s) {
	this->bot.x += s;
}

void Rectangle::translateRight(float s) {
	this->top.x += s;
}

void Rectangle::translateTop(float s) {
	this->top.y += s;
}

void Rectangle::translateBottom(float s) {
	this->bot.y += s;
}

void Rectangle::setHeight(float h) {
	this->bot.y = this->top.y - h;
}

void Rectangle::setWidth(float w) {
	this->top.x = this->bot.x + w;
}

void Rectangle::setSize(glm::vec2 size) {
	this->setWidth(size.x);
	this->setHeight(size.y);
}

float Rectangle::getWidth() const {
	return this->top.x - this->bot.x;
}

float Rectangle::getHeight() const {
	return this->top.y - this->bot.y;
}

void ScreenRectangle::setPixelSize(glm::ivec2 px) {
	this->screenPixels = px;
}

glm::ivec2 ScreenRectangle::getPixelSize() const {
	return this->screenPixels;
}

glm::ivec2 ScreenRectangle::getPixelPos() const {
	// TODO
	assert(false);
	return glm::ivec2(0);
}

void Rectangle::setTopLeft(glm::vec2 p) {
	this->bot.x = p.x;
	this->top.y = p.y;
}

void Rectangle::setBottomRight(glm::vec2 p) {
	this->bot.y = p.y;
	this->top.x = p.x;
}

void Rectangle::setTopRight(glm::vec2 p) {
	this->top = p;
}

void Rectangle::setBottomLeft(glm::vec2 p) {
	this->bot = p;
}
