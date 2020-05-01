#include "common.h"
#include "Renderer.h"
#include "GameState.h"
#include "CameraInfo.h"
//#include "WindowManager.h"
#include "FPSLimiter.h"
//#include "GameObject.h"
#include "RenderInfo.h"

void Renderer::prepareRender(RenderInfo& renderInfo, GameState& gameState, WindowManager& windowManager) {
	//for (auto& object : gameState.objects) {
	//	object->renderPrepare(target.renderState);
	//}
	//for (auto& t : gameState.inventory.worldSelector.selected) {
	//	t.get()->appendSelectionInfo(gameState, target);
	//}
	//
	//glm::vec2 p = glm::floor(gameState.playerCursor);
	//auto res = gameState.staticWorld.getBlock(p);
	//if (res.first == 1) {
	//	res.second.get()->appendSelectionInfo(gameState, target);
	//}

	gameState.appendStaticRenderInfo(renderInfo);
	//windowManager.addRenderInfo(target.uiRenderInfo, target.cameraInfo, textRenderer);
}

void Renderer::render(GLFWwindow* window, RenderInfo& renderInfo, bool differentContext) {
	if (differentContext) {
		glfwMakeContextCurrent(window);
	}
	//gameState.inventory.worldSelector.dummyRender(gameState);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//shadowRenderer.lightsToShadowMap.clearBuffer();
	//if (shadowOption) {
	//	shadowRenderer.lightsToShadowMap.render(
	//		renderInfo.renderState.lightsInfo,
	//		renderInfo.renderState.middlesInfo
	//	);

	//	shadowRenderer.lightsToShadowMapStatic.render(
	//		renderInfo.renderState.lightsInfo,
	//		renderInfo.staticWorldRenderInfo,
	//		shadowRenderer.lightsToShadowMap.depthBuffer
	//	);
	//}

	//shadowRenderer.shadowToScreen.render(
	//	renderInfo.renderState.lightsInfo,
	//	renderInfo.renderState.middlesInfo,
	//	shadowRenderer.lightsToShadowMap.depthTexture,
	//	lightScreenBuffer,
	//	renderInfo.cameraInfo
	//);
	//mainRenderer.render(renderInfo.renderState.backsInfo, 0, renderInfo.cameraInfo);
	//mainRenderer.render(renderInfo.renderState.middlesInfo, 0, renderInfo.cameraInfo);

	staticWorldRenderer.render(renderInfo.staticWorldRenderInfo, 0, renderInfo.cameraInfo);
	//staticWorldRenderer.render(staticWorldRenderInfos, 0, camInfo);

	//passthroughRenderer.renderTextureToTarget(
	//	lightScreenBuffer.screenTexture[0].ID,
	//	lightScreenBufferSmall.screenBuffer[0].ID,
	//	0, 0, lightScreenBufferSmall.size, lightScreenBufferSmall.size
	//);

	//glm::ivec2 b = blurOption;
	//int fullBlur = glm::clamp(b.x, 0, 5);
	//int downscaledBlur = glm::clamp(b.y, 0, 5);
	//if (fullBlur + downscaledBlur != 0) {
	//	blur.render(
	//		fullBlur,
	//		0, 0, lightScreenBuffer.size, lightScreenBuffer.size,
	//		lightScreenBuffer.screenBuffer,
	//		lightScreenBuffer.screenTexture
	//	);
	//	blur.render(
	//		downscaledBlur,
	//		0, 0, lightScreenBufferSmall.size, lightScreenBufferSmall.size,
	//		lightScreenBufferSmall.screenBuffer,
	//		lightScreenBufferSmall.screenTexture
	//	);
	//}

	//auto screen = std::make_unique<bwo::Buffer>();

	//lightScreenBufferFinal.clear(ambientLightOption);
	//lightScreenBufferSmall.render(
	//	0, 0, lightScreenBufferFinal.size, lightScreenBufferFinal.size,
	//	lightScreenBufferFinal.screenBuffer[0],
	//	true
	//);
	//lightScreenBuffer.render(
	//	0, 0, lightScreenBufferFinal.size, lightScreenBufferFinal.size,
	//	lightScreenBufferFinal.screenBuffer[0],
	//	true
	//);

	//lightScreenBufferFinal.render(
	//	0, 0, renderInfo.cameraInfo.x, renderInfo.cameraInfo.y,
	//	*screen.get(),
	//	false
	//);

	//selectionRenderer.render(renderInfo, 0);

	//if (debugOption) {
	//	DebugRenderer().render(0, renderInfo.cameraInfo, renderInfo.debugPoints, renderInfo.debugLines);
	//}

	//if (uiOption) {
	//	uiBackgroundRenderer.render(renderInfo.uiRenderInfo, screen.get()->ID, renderInfo.cameraInfo);
	//	textRenderer.render(renderInfo.uiRenderInfo.textRenderInfo, screen.get()->ID, renderInfo.cameraInfo);
	//}

	glfwSwapBuffers(window);

	if (differentContext) {
		glfwMakeContextCurrent(0);
	}
}

Renderer::Renderer() 
	// :
	//shadowRenderer(256 * 4, 256 * 4),
	//lightScreenBuffer(9),
	//lightScreenBufferSmall(6),
	//lightScreenBufferFinal(10),
	//textRenderer("Consolas_60x111_15x9.dds", { 60,111 }, { 15,9 }) 
{
}

Renderer::~Renderer() {
}
