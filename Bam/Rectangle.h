#pragma once
struct Rectangle
{
public:
	glm::vec2 bot;
	glm::vec2 top;

	bool contains(glm::vec2 p);
	bool contains(Rectangle& rec);

	float getLeft();
	float getRight();
	float getTop();
	float getBot();
};

