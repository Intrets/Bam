#pragma once

#include "common.h"

#include "UIrenderInfo.h"
#include "CameraInfo.h"
#include "DebugRenderInfo.h"
#include "SelectionRenderInfo.h"
#include "TextRenderInfo.h"
#include "StaticWorldRenderInfo.h"

struct RenderInfo
{
	glm::ivec2 frameSize;
	CameraInfo cameraInfo;
	StaticWorldRenderInfo staticWorldRenderInfo;
	DebugRenderInfo debugRenderInfo;
	SelectionRenderInfo selectionRenderInfo;
	TextRenderInfo textRenderInfo;
	UIRenderInfo uiRenderInfo;
};
