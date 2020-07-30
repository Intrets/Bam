#include "common.h"

#include "Rectangle.h"

bool Rectangle::contains(glm::vec2 p) {
	return (this->bot.x < p.x) && (p.x < this->top.x) && (this->bot.y < p.y) && (p.y < this->top.y);
}

bool Rectangle::contains(Rectangle& rec) {
	return this->contains(rec.bot) && this->contains(rec.top);
}

float Rectangle::getLeft() {
	return bot.x;
}

float Rectangle::getRight() {
	return top.x;
}

float Rectangle::getTop() {
	return top.y;
}

float Rectangle::getBot() {
	return bot.y;
}
