#pragma once

//#include "ShadowRenderer.h"
//#include "MainRenderer.h"
#include "StaticWorldRenderer.h"
//#include "Blur.h"
//#include "TextRenderer.h"
//#include "UIbackgroundRenderer.h"
#include "Option.h"
//#include "SelectionHighlightRenderer.h"
#include "DebugRenderer.h"

struct RenderInfo;
class FPSLimiter;
class GameState;
class WindowManager;

class Renderer
{
private:

	//Option<bool> shadowOption{ "render_shadow", true };
	//Option<glm::ivec2> blurOption{ "render_blur", { 1, 2 } };
	//Option<bool> uiOption{ "render_ui" , true };
	Option<OPTIONS2::GR_DEBUG, bool> debugOption;
	//Option<glm::vec4> ambientLightOption{ "cosm_ambient_light", glm::vec4(0.3) };

	DebugRenderer debugRenderer;

	//ShadowRenderer shadowRenderer;

	//MainRenderer mainRenderer;
	//PassthroughRenderer passthroughRenderer;

	StaticWorldRenderer staticWorldRenderer;
	//TextRenderer textRenderer;
	//UIbackgroundRenderer uiBackgroundRenderer;
	//SelectionHighlightRenderer selectionRenderer;

	//Blur blur;

	//LightScreenBuffer lightScreenBuffer;
	//LightScreenBuffer lightScreenBufferSmall;
	//LightScreenBuffer lightScreenBufferFinal;

public:
	void prepareRender(GLFWwindow* window, RenderInfo& target, GameState& gameState, WindowManager& windowManager);

	void render(GLFWwindow* window, RenderInfo& renderInfo);

	Renderer();
	~Renderer();
};

