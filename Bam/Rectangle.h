#pragma once
struct Rectangle
{
public:
	glm::vec2 bot;
	glm::vec2 top;

	bool contains(glm::vec2 p) const ;
	bool contains(Rectangle& rec) const ;

	float getLeft() const;
	float getRight() const ;
	float getTop() const ;
	float getBot() const ;
};

