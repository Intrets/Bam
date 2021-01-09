#pragma once

#include <string>

struct ShapeData
{
	std::string name = "block";
	int32_t texture;
};

void loadShapes();
