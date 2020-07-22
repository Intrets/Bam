#include "common.h"
#include "Renderer.h"
#include "CameraInfo.h"
//#include "WindowManager.h"
#include "FPSLimiter.h"
//#include "GameObject.h"
#include "RenderInfo.h"
#include "State.h"
#include "BlitRenderer.h"
#include "Fonts.h"

void Renderer::prepareRender(GLFWwindow* window, RenderInfo& renderInfo, State& state) {
	auto& gameState = state.gameState;
	auto& uiState = state.uiState;

	int32_t frameSizeX, frameSizeY;
	glfwGetFramebufferSize(window, &frameSizeX, &frameSizeY);
	float ratio = frameSizeX / static_cast<float>(frameSizeY);
	glm::vec2 viewport(ratio, 1.0f);
	Option<OPTIONS2::CL_VIEWPORTSCALE, float> viewportScale;
	viewport *= viewportScale.getVal();
	renderInfo.frameSize = { frameSizeX, frameSizeY };
	renderInfo.cameraInfo = { frameSizeX, frameSizeY, state.player.getCameraPosition(), glm::vec3(viewport, 200.0f) };

	state.player.selection.addRenderInfo(gameState, renderInfo, 0);

	renderInfo.debugRenderInfo = *Locator<DebugRenderInfo>::get();
	Locator<DebugRenderInfo>::provide(new DebugRenderInfo());

	gameState.appendStaticRenderInfo(renderInfo);

	uiState.appendRenderInfo(gameState, renderInfo);
}

void Renderer::render(GLFWwindow* window, RenderInfo& renderInfo) {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	staticWorldRenderer.render(renderInfo.staticWorldRenderInfo, 0, renderInfo.cameraInfo);

	selectionRenderer.render(renderInfo, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_DEPTH_BUFFER_BIT);

	uiBackgroundRenderer.render(renderInfo.uiRenderInfo, 0, renderInfo.cameraInfo);

	textRenderer.render(renderInfo.textRenderInfo, Locator<Fonts>::ref(), 0, renderInfo.cameraInfo);

	if (debugOption.getVal()) {
		debugRenderer.render(0, renderInfo);
	}

	glfwSwapBuffers(window);
}

Renderer::Renderer() {
}

Renderer::~Renderer() {
}
