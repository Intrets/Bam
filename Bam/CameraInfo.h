#pragma once

#include "common.h"

struct CameraInfo
{
	int x;
	int y;
	glm::vec2 camPos;
	glm::vec3 viewPort;
	glm::mat4 VP = glm::ortho(-viewPort.x, viewPort.x, -viewPort.y, viewPort.y, -viewPort.z, viewPort.z) * glm::translate(-glm::vec3(camPos, 0.0f));
};