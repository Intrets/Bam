#pragma once

#include "TextRenderer.h"
#include "UIbackgroundRenderer.h"
#include "DebugRenderer.h"
#include "SelectionHighlightRenderer.h"
#include "BlockRenderer.h"

struct PlayerState;
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
	void prepareRender(GLFWwindow* window, RenderInfo& target, PlayerState& playerState);

	void render(GLFWwindow* window, RenderInfo const& renderInfo);

	Renderer();
	~Renderer();
};

