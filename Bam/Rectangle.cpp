#include "common.h"

#include "Rectangle.h"

bool Rectangle::contains(glm::vec2 p) const {
	return (this->bot.x < p.x) && (p.x < this->top.x) && (this->bot.y < p.y) && (p.y < this->top.y);
}

bool Rectangle::contains(Rectangle& rec) const {
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
