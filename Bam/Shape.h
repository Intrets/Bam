#pragma once

#include "Activity.h"

struct ShapeData
{
	int32_t texture;

};

class Shape
{
private:
	static std::vector<ShapeData> shapeData;
	static std::unordered_map<std::string, int32_t> nameMap;

	int32_t index;
	ShapeData const& getShapeData() const;

public:
	static void loadShapes();

};