#pragma once

#include "TextRenderer.h"
#include "UIbackgroundRenderer.h"
#include "DebugRenderer.h"
#include "SelectionHighlightRenderer.h"
#include "BlockRenderer.h"

struct State;
struct RenderInfo;
class FPSLimiter;
class WindowManager;

class Renderer
{
private:
	DebugRenderer debugRenderer;

	BlockRenderer blockRenderer;
	TextRenderer textRenderer;
	UIbackgroundRenderer uiBackgroundRenderer;
	SelectionHighlightRenderer selectionRenderer;

public:
	void prepareRender(GLFWwindow* window, RenderInfo& target, State& state);

	void render(GLFWwindow* window, RenderInfo const& renderInfo);

	Renderer();
	~Renderer();
};

