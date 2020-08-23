#pragma once

struct Rectangle;

struct ScreenRectangle
{
	glm::ivec2 screenPixels;
	glm::vec2 bot;
	glm::vec2 top;

	glm::ivec2 getPixelSize() const ;
	glm::ivec2 getPixelPos() const ;
	bool contains(glm::vec2 p) const ;
	void scaleAnchorTopLeft(glm::vec2 s);

	void set(glm::vec2 p1, glm::vec2 p2);
	void set(Rectangle rec);

	void setTopLeft(glm::vec2 p);
	void setBottomRight(glm::vec2 p);
	void setTopRight(glm::vec2 p);
	void setBottomLeft(glm::vec2 p);

	void translate(glm::vec2 p);
	void setHeight(float h);
	void setWidth(float w);

	bool equals(ScreenRectangle& other) const ;

	glm::vec2 getTopLeft() const;
	glm::vec2 getBottomRight() const;
	glm::vec2 getTopRight() const;
	glm::vec2 getBottomLeft() const;

	float getWidth() const ;
	float getHeight() const ;

	glm::vec2 size() const;
};

