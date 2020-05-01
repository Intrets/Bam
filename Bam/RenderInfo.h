#pragma once

#include "common.h"

//#include "UIrenderInfo.h"
#include "StaticWorldRenderer.h"
#include "CameraInfo.h"
#include "DebugRenderInfo.h"
//#include "SelectionRenderInfo.h"
//#include "RenderState.h"

struct RenderInfo
{
	CameraInfo cameraInfo;
	StaticWorldRenderInfo staticWorldRenderInfo;
	DebugRenderInfo debugRenderInfo;
	//SelectionRenderInfo selectionRenderInfo;
	//UIrenderInfo uiRenderInfo;
	//RenderState renderState;

	std::vector<glm::vec2> debugPoints;
	std::vector<std::vector<glm::vec2>> debugLines;

	//TODO: add buffering of options
	// eg OptionInteger blurRender;
};
