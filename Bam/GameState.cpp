#include "common.h"
#include "StaticWorldChunk.h"
#include "GameState.h"
#include "RenderInfo.h"
#include "Option.h"

#include "Platform.h"

glm::vec2 GameState::getPlayerCursorWorldSpace() {
	return playerCursorWorldSpace;
}

glm::vec2 GameState::getPlayerCursorScreenSpace() {
	return playerCursorScreenSpace;
}

glm::vec2 GameState::getPlayerCursorWorldSpaceD() {
	return playerCursorWorldSpaceD;
}

glm::vec2 GameState::getPlayerCursorScreenSpaceD() {
	return playerCursorScreenSpaceD;
}

void GameState::updatePlayerCursorScreenSpace(GLFWwindow* window) {
	double x;
	double y;
	glfwGetCursorPos(window, &x, &y);

	int32_t frameSizeX, frameSizeY;
	glfwGetFramebufferSize(window, &frameSizeX, &frameSizeY);
	x = x / frameSizeX;
	y = y / frameSizeY;
	y = 1 - y;
	x = 2 * x - 1;
	y = 2 * y - 1;

	float ratio = frameSizeX / static_cast<float>(frameSizeY);
	glm::vec2 viewport(ratio, 1.0f);
	Option<OPTIONS2::CL_VIEWPORTSCALE, float> viewportScale;
	viewport *= viewportScale.getVal();

	auto newScreenSpace = glm::vec2(x, y);
	playerCursorScreenSpaceD = newScreenSpace - playerCursorScreenSpace;
	playerCursorScreenSpace = newScreenSpace;

	playerCursorWorldSpace = playerPos + playerCursorScreenSpace * viewport;
	playerCursorWorldSpaceD = playerPos + playerCursorScreenSpaceD * viewport;
}

bool GameState::load(Loader& loader) {
	loader.retrieve(tick);
	staticWorld.load(loader);
	activityPaceHandler.load(loader);
	movementPaceHandler.load(loader);
	return false;
}

bool GameState::save(Saver& saver) {
	saver.store(tick);
	staticWorld.save(saver);
	activityPaceHandler.save(saver);
	movementPaceHandler.save(saver);
	return false;
}

void GameState::appendStaticRenderInfo(RenderInfo& renderInfo) {
	staticWorld.appendStaticRenderInfo(renderInfo);
	auto activityManager = Locator<ReferenceManager<Activity>>::getService();
	for (auto& p : activityManager->data) {
		p.second.get()->appendStaticRenderInfo(*this, renderInfo.staticWorldRenderInfo);
	}
}

GameState::GameState() {
	WeakReference<Activity, Platform>(new Platform(0, *this, glm::vec2(3), glm::vec2(3)));
}


GameState::~GameState() {
}
