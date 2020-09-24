#include "common.h"

#include "Shape.h"
#include "Platform.h"

std::vector<ShapeData> Shape::shapeData{};
std::unordered_map<std::string, int32_t> Shape::nameMap{};

ShapeData const& Shape::getShapeData() const {
	return Shape::shapeData[this->index];
}

void Shape::loadShapes() {
}

