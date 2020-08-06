#include "common.h"

#include "Main.h"
#include "GameLogic.h"
#include "Renderer.h"
#include "FPSLimiter.h"
#include "GameState.h"
#include "RenderInfo.h"
#include "ControlState.h"
#include <thread>
#include "FPSDisplay.h"
#include "UIState.h"
#include "State.h"
#include "Timer.h"

ControlState controlState;

static void key_callback(GLFWwindow* w, int32_t key, int32_t scancode, int32_t action, int32_t mods) {
	controlState.key_callback(w, key, scancode, action, mods);
}

static void mouse_callback(GLFWwindow* w, int32_t key, int32_t action, int32_t mods) {
	key_callback(w, key + GLFW_KEY_LAST, 0, action, mods);
}

void char_callback(GLFWwindow* w, unsigned int character) {
	controlState.char_callback(w, character);
}

void scroll_callback(GLFWwindow* w, double xoffset, double yoffset) {
	controlState.scroll_callback(w, xoffset, yoffset);
}

void mainLoop(GLFWwindow* window) {
	glfwSetCharCallback(window, char_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	State state(controlState);
	state.uiState.updateSize(window);

	FPSLimiter fpsLimiter;
	FPSDisplay fpsDisplay;

	GameLogic gameLogic;

	Renderer renderer;

	std::thread logicThread;

	while (!glfwWindowShouldClose(window)) {
		RenderInfo renderInfo;

		bool rendering = fpsLimiter.ready();

		if (rendering) {
			Locator<Timer>::ref().endTiming("Render Loop");
			Locator<Timer>::ref().newTiming("Render Loop");

			Locator<Timer>::ref().newTiming("Prepare render");
			state.uiState.updateSize(window);
			renderer.prepareRender(window, renderInfo, state);
			Locator<Timer>::ref().endTiming("Prepare render");
		}

		if (gameLogic.ready()) {
			Locator<Timer>::ref().endTiming("Game Loop");
			Locator<Timer>::ref().newTiming("Game Loop");

			Locator<Timer>::ref().newTiming("UI Logic");
			state.controlState.cycleStates();
			glfwPollEvents();

			state.uiState.updateCursor(window, state.player.getCameraPosition());

			state.uiState.run(state);
			Locator<Timer>::ref().endTiming("UI Logic");

			Locator<Timer>::ref().newTiming("Game Logic");
			if (Option<GR_RENDERTHREAD, bool>::getVal()) {
				logicThread = std::thread(&GameLogic::runStep, &gameLogic, std::ref(state.gameState));
			}
			else {
				gameLogic.runStep(state.gameState);
				Locator<Timer>::ref().endTiming("Game Logic");
			}
		}

		if (rendering) {
			Locator<Timer>::ref().newTiming("Render");
			fpsLimiter.renderStart();
			renderer.render(window, renderInfo);
			fpsLimiter.renderFinish();
			fpsDisplay.displayFPS(window);
			Locator<Timer>::ref().endTiming("Render");
		}

		if (logicThread.joinable()) {
			logicThread.join();
			Locator<Timer>::ref().endTiming("Game Logic");
		}

		{
			GLenum err;
			while ((err = glGetError()) != GL_NO_ERROR) {
				Locator<Log>::ref() << Log::OPEN{} << "OpenGL ERROR: " << std::hex << err << std::dec << "\n" << Log::CLOSE{};
				std::cout << "OpenGL ERROR: " << std::hex << err << std::dec << "\n";
			}
		}

		std::this_thread::sleep_for(std::chrono::microseconds((long) 500));
	}
}