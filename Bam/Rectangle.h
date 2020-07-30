#pragma once
struct Rectangle
{
public:
	glm::vec2 bot;
	glm::vec2 top;

	bool contains(glm::vec2 p);
	bool contains(Rectangle& rec);

	float left1();
	float right1();
	float top1();
	float bot1();
};

