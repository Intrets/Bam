#pragma once

#include "common.h"

#include "UIrenderInfo.h"
#include "StaticWorldRenderer.h"
#include "CameraInfo.h"
#include "DebugRenderInfo.h"
#include "SelectionRenderInfo.h"
#include "TextRenderInfo.h"
//#include "RenderState.h"

struct RenderInfo
{
	glm::ivec2 frameSize;
	CameraInfo cameraInfo;
	StaticWorldRenderInfo staticWorldRenderInfo;
	DebugRenderInfo debugRenderInfo;
	SelectionRenderInfo selectionRenderInfo;
	TextRenderInfo textRenderInfo;
	UIRenderInfo uiRenderInfo;

	//TODO: add buffering of options
	// eg OptionInteger blurRender;
};
