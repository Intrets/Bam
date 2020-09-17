#include "common.h"
#include "Renderer.h"
#include "CameraInfo.h"
#include "RenderInfo.h"
#include "State.h"
#include "BlitRenderer.h"
#include "Fonts.h"
#include "Option.h"
#include "Timer.h"
#include "BlitRendererArrayTexture.h"
#include "BlockIDTextures.h"

void Renderer::prepareRender(GLFWwindow* window, RenderInfo& renderInfo, State& state) {
	auto& gameState = state.gameState;
	auto& uiState = state.uiState;

	int32_t frameSizeX, frameSizeY;
	glfwGetFramebufferSize(window, &frameSizeX, &frameSizeY);
	float ratio = frameSizeX / static_cast<float>(frameSizeY);
	glm::vec2 viewport(ratio, 1.0f);
	viewport *= Option<OPTION::CL_VIEWPORTSCALE, float>::getVal();
	renderInfo.frameSize = { frameSizeX, frameSizeY };
	renderInfo.cameraInfo = { frameSizeX, frameSizeY, state.player.getCameraPosition(), glm::vec3(viewport, 200.0f) };

	Locator<Timer>::ref().newTiming("Prepare Debug");
	renderInfo.debugRenderInfo = *Locator<DebugRenderInfo>::get();
	Locator<DebugRenderInfo>::provide(new DebugRenderInfo());
	Locator<Timer>::ref().endTiming("Prepare Debug");

	gameState.appendStaticRenderInfo(renderInfo);

	Locator<Timer>::ref().newTiming("Prepare UI");
	uiState.appendRenderInfo(gameState, renderInfo);
	Locator<Timer>::ref().endTiming("Prepare UI");
}

void Renderer::render(GLFWwindow* window, RenderInfo const& renderInfo) {
	Locator<Timer>::ref().newTiming("Render");

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//this->staticWorldRenderer.render(renderInfo.staticWorldRenderInfo, 0, renderInfo.cameraInfo);

	Locator<BlitRendererArrayTexture>::get()->render(
		renderInfo.staticWorldRenderInfo.offsets,
		renderInfo.staticWorldRenderInfo.rotations,
		renderInfo.staticWorldRenderInfo.textureIDs,
		0,
		{ 0,0, renderInfo.cameraInfo.x, renderInfo.cameraInfo.y },
		Locator<BlockIDTextures>::ref().getTextureArrayID(),
		std::nullopt,
		renderInfo.cameraInfo.VP
	);


	this->selectionRenderer.render(renderInfo, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_DEPTH_BUFFER_BIT);

	this->uiBackgroundRenderer.render(renderInfo.uiRenderInfo, 0, renderInfo.cameraInfo);

	this->textRenderer.render(renderInfo.textRenderInfo, Locator<Fonts>::ref(), 0, renderInfo.cameraInfo);

	if (Option<OPTION::GR_DEBUG, bool>::getVal()) {
		this->debugRenderer.render(0, renderInfo);
	}
	Locator<Timer>::ref().endTiming("Render");

	Locator<Timer>::ref().newTiming("Swap Buffers");
	glfwSwapBuffers(window);
	Locator<Timer>::ref().endTiming("Swap Buffers");
}

Renderer::Renderer() {
}

Renderer::~Renderer() {
}
