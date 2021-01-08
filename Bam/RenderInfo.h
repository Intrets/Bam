#pragma once

#include <GLM/glm.hpp>

#include "UIRenderInfo.h"
#include "CameraInfo.h"
#include "DebugRenderInfo.h"
#include "SelectionRenderInfo.h"
#include "TextRenderInfo.h"
#include "StaticWorldRenderInfo.h"

struct RenderInfo
{
	int32_t playerIndex;
	glm::ivec2 frameSize;
	CameraInfo cameraInfo;
	StaticWorldRenderInfo staticWorldRenderInfo;
	DebugRenderInfo debugRenderInfo;
	SelectionRenderInfo selectionRenderInfo;
	TextRenderInfo textRenderInfo;
	UIRenderInfo uiRenderInfo;
};
