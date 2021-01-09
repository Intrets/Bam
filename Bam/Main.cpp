#include "common.h"

#include "Main.h"

#include "NetworkLayer.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "GameLogic.h"
#include "Renderer.h"
#include "GameState.h"
#include "RenderInfo.h"
#include "ControlState.h"
#include <thread>
#include "UIState.h"
#include "PlayerState.h"
#include "Timer.h"
#include "Option.h"
#include "TickLimiter.h"
#include "RenderLimiter.h"
#include "Loader.h"
#include "Saver.h"
#include "Locator.h"
#include "Log.h"
#include "PathManager.h"

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

struct Client
{
	Renderer renderer;
	UIState uiState;
	RenderLimiter renderLimiter;
	PlayerState state;

	Client(GameState& gameState) :
		state({ gameState.getPlayer(0).value(), gameState, controlState, uiState }) {
	};
};

void mainLoop(GLFWwindow* window, PROGRAM::TYPE type) {
	NETWORK::Network network;

	switch (type) {
		case PROGRAM::TYPE::OFFLINE:
		case PROGRAM::TYPE::CLIENT:
			break;
		case PROGRAM::TYPE::SERVER:
			network.initializeServer(27015);
			break;
		case PROGRAM::TYPE::HEADLESS_SERVER:
			network.initializeServer(27015);
			break;
		default:
			break;
	}

	switch (type) {
		case PROGRAM::TYPE::OFFLINE:
		case PROGRAM::TYPE::CLIENT:
		case PROGRAM::TYPE::SERVER:
			glfwSetCharCallback(window, char_callback);
			glfwSetKeyCallback(window, key_callback);
			glfwSetMouseButtonCallback(window, mouse_callback);
			glfwSetScrollCallback(window, scroll_callback);
			break;
		case PROGRAM::TYPE::HEADLESS_SERVER:
			break;
		default:
			break;
	}

	GameState gameState;
	gameState.makePlayer();

	GameLogic gameLogic;

	TickLimiter tickLimiterGameLogic;

	std::unique_ptr<Client> client;
	switch (type) {
		case PROGRAM::TYPE::OFFLINE:
		case PROGRAM::TYPE::CLIENT:
		case PROGRAM::TYPE::SERVER:
			client = std::make_unique<Client>(gameState);
			client->uiState.updateSize(window);
			break;
		case PROGRAM::TYPE::HEADLESS_SERVER:
			break;
		default:
			break;
	}

	std::thread logicThread;

	bool stateChanged = false;

	RenderInfo renderInfo;

	while (!glfwWindowShouldClose(window)) {

		if (client->state.uiState.shouldReset()) {
			client->state.uiState.clear();
			client->state.uiState.init();
		}
		if (client->state.gameState.loadFile.has_value()) {
			auto const& name = client->state.gameState.loadFile.value();
			Locator<Log>::ref().putLine("loading: " + name);

			std::ifstream save;
			Locator<PathManager>::ref().openSave(save, name);

			Loader(save, client->state.gameState).loadGame();

			client->state.player = gameState.getPlayer(0).value();
			client->state.gameState.loadFile = std::nullopt;
		}
		else if (client->state.gameState.saveFile.has_value()) {
			auto const& name = client->state.gameState.saveFile.value();
			Locator<Log>::ref().putLine("saving: " + name);

			std::ofstream save;
			Locator<PathManager>::ref().openSave(save, name);

			Saver(save, client->state.gameState).saveGame();
			client->state.gameState.saveFile = std::nullopt;
		}

		bool rendering = false;
		if (stateChanged && client->renderLimiter.ready()) {
			client->renderLimiter.advance();
			stateChanged = false;
			rendering = true;

			Locator<Timer>::ref().endTiming("Render Loop");
			Locator<Timer>::ref().newTiming("Render Loop");

			Locator<Timer>::ref().newTiming("UI update size");
			client->state.uiState.updateSize(window);
			Locator<Timer>::ref().endTiming("UI update size");

			Locator<Timer>::ref().newTiming("Prepare render");
			renderInfo = RenderInfo();
			client->renderer.prepareRender(window, renderInfo, client->state);
			Locator<Timer>::ref().endTiming("Prepare render");

			Locator<Timer>::ref().newTiming("Polling");
			client->state.controlState.cycleStates();
			glfwPollEvents();
			Locator<Timer>::ref().endTiming("Polling");

			Locator<Timer>::ref().newTiming("UI Logic");
			client->state.uiState.updateCursor(window, client->state.getPlayer().getCameraPosition());

			client->state.uiState.run(client->state);
			Locator<Timer>::ref().endTiming("UI Logic");
		}

		const static auto gameTick = [&]()
		{
			for (int32_t i = 0; i < 10; i++) {
				if (!tickLimiterGameLogic.ready()) {
					break;
				}
				stateChanged = true;
				gameLogic.runStep(client->state.gameState);
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
			client->renderer.render(window, renderInfo);

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