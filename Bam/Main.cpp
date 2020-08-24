#include "common.h"

#include "Main.h"

#include "GameLogic.h"
#include "Renderer.h"
#include "GameState.h"
#include "RenderInfo.h"
#include "ControlState.h"
#include <thread>
#include "UIState.h"
#include "State.h"
#include "Timer.h"
#include "Option.h"
#include "TickLimiter.h"
#include "RenderLimiter.h"

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

	GameLogic gameLogic;

	TickLimiter tickLimiterGameLogic;
	RenderLimiter tickLimiterRender;

	Renderer renderer;

	std::thread logicThread;

	bool stateChanged = false;

	while (!glfwWindowShouldClose(window)) {
		RenderInfo renderInfo;

		bool rendering = false;
		if (stateChanged && tickLimiterRender.ready()) {
			tickLimiterRender.advance();
			stateChanged = false;
			rendering = true;

			Locator<Timer>::ref().endTiming("Render Loop");
			Locator<Timer>::ref().newTiming("Render Loop");

			Locator<Timer>::ref().newTiming("UI update size");
			state.uiState.updateSize(window);
			Locator<Timer>::ref().endTiming("UI update size");
			Locator<Timer>::ref().newTiming("Prepare render");
			renderer.prepareRender(window, renderInfo, state);
			Locator<Timer>::ref().endTiming("Prepare render");

			Locator<Timer>::ref().newTiming("UI Logic");
			state.controlState.cycleStates();
			glfwPollEvents();

			state.uiState.updateCursor(window, state.player.getCameraPosition());

			state.uiState.run(state);
			Locator<Timer>::ref().endTiming("UI Logic");
		}

		const static auto gameTick = [&]()
		{
			for (int32_t i = 0; i < 10; i++) {
				if (!tickLimiterGameLogic.ready()) {
					break;
				}
				stateChanged = true;
				gameLogic.runStep(state.gameState);
				tickLimiterGameLogic.advance();
				Locator<Timer>::ref().endTiming("Game Loop");
				Locator<Timer>::ref().newTiming("Game Loop");
			}
		};

		if (Option<OPTION::GR_RENDERTHREAD, bool>::getVal()) {
			logicThread = std::thread(gameTick);
		}
		else {
			gameTick();
		}

		if (rendering) {
			rendering = false;
			renderer.render(window, renderInfo);

			GLenum err;
			while ((err = glGetError()) != GL_NO_ERROR) {
				Locator<Log>::ref().putStreamLine(std::stringstream() << "OpenGL ERROR: " << std::hex << err << std::dec);
				std::cout << "OpenGL ERROR: " << std::hex << err << std::dec << "\n";
			}
		}

		if (logicThread.joinable()) {
			logicThread.join();
		}
	}
}