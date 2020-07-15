#pragma once

class DebugRenderInfo
{
public:
	void addBox(glm::vec2 u, glm::vec2 d);
	void addBoxScreen(glm::vec2 u, glm::vec2 d);
	void addPoint(glm::vec2 p);
	void addLine(glm::vec2 p1, glm::vec2 p2);

private:
	friend class DebugRenderer;
	std::vector<glm::vec2> points;
	std::vector<std::vector<glm::vec2>> lines;

	std::vector<std::vector<glm::vec2>> linesScreen;
};

