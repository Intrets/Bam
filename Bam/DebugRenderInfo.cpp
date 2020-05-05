#include "common.h"
#include "DebugRenderInfo.h"

void DebugRenderInfo::addBox(glm::vec2 u, glm::vec2 d) {
	lines.push_back(std::vector<glm::vec2>());
	lines.back().push_back(u);
	lines.back().push_back({ u.x, d.y });
	lines.back().push_back(d);
	lines.back().push_back({ d.x, u.y });
	lines.back().push_back(u);
}

void DebugRenderInfo::addPoint(glm::vec2 p) {
	points.push_back(p);
}

void DebugRenderInfo::addLine(glm::vec2 p1, glm::vec2 p2) {
	std::vector<glm::vec2> line;
	line.push_back(p1);
	line.push_back(p2);
	lines.push_back(line);
}
