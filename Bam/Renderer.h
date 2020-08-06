#pragma once

#include "StaticWorldRenderer.h"
#include "TextRenderer.h"
#include "UIbackgroundRenderer.h"
#include "Option.h"
#include "DebugRenderer.h"
#include "SelectionHighlightRenderer.h"

struct State;
struct RenderInfo;
class FPSLimiter;
class WindowManager;

class Renderer
{
private:
	DebugRenderer debugRenderer;

	StaticWorldRenderer staticWorldRenderer;
	TextRenderer textRenderer;
	UIbackgroundRenderer uiBackgroundRenderer;
	SelectionHighlightRenderer selectionRenderer;

public:
	void prepareRender(GLFWwindow* window, RenderInfo& target, State& state);

	void render(GLFWwindow* window, RenderInfo& renderInfo);

	Renderer();
	~Renderer();
};

