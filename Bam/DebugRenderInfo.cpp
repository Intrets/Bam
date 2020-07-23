#include "common.h"
#include "DebugRenderInfo.h"

void DebugRenderInfo::addBoxScreen(glm::vec2 u, glm::vec2 d) {
	this->linesScreen.push_back(std::vector<glm::vec2>());
	this->linesScreen.back().push_back(u);
	this->linesScreen.back().push_back({ u.x, d.y });
	this->linesScreen.back().push_back(d);
	this->linesScreen.back().push_back({ d.x, u.y });
	this->linesScreen.back().push_back(u);
}

void DebugRenderInfo::addBox(glm::vec2 u, glm::vec2 d) {
	this->lines.push_back(std::vector<glm::vec2>());
	this->lines.back().push_back(u);
	this->lines.back().push_back({ u.x, d.y });
	this->lines.back().push_back(d);
	this->lines.back().push_back({ d.x, u.y });
	this->lines.back().push_back(u);
}

void DebugRenderInfo::addPoint(glm::vec2 p) {
	this->points.push_back(p);
}

void DebugRenderInfo::addLine(glm::vec2 p1, glm::vec2 p2) {
	std::vector<glm::vec2> line;
	line.push_back(p1);
	line.push_back(p2);
	this->lines.push_back(line);
}
