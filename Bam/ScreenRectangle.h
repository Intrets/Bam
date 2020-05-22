#pragma once
struct ScreenRectangle
{
	glm::ivec2 screenPixels;
	glm::vec2 bot;
	glm::vec2 top;

	glm::ivec2 getPixelSize();
	glm::ivec2 getPixelPos();
	bool contains(glm::vec2 p);
	void scaleAnchorTopLeft(glm::vec2 s);

	void set(glm::vec2 p1, glm::vec2 p2);
	void setTopLeft(glm::vec2 p);
	void setBottomRight(glm::vec2 p);
	void translate(glm::vec2 p);

	glm::vec2 topLeft();
	glm::vec2 bottomRight();
	glm::vec2 topRight();
};

