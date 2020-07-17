#pragma once

#include "ActivitySelector.h"

class Player
{
public:
	ActivitySelector selection;

	glm::vec2 pos;

	glm::vec2 getCameraPosition();
};

