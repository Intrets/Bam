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
#include "Coordinator.h"
#include "Network.h"
#include "Uuid.h"
#include "MetaOperation.h"
#include "Client.h"

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

//struct Client
//{
//	Renderer renderer;
//	UIState uiState;
//	RenderLimiter renderLimiter;
//	PlayerState state;
//
//	Client(GameState& gameState) :
//		state({ gameState.getPlayer(0).value(), gameState, controlState, uiState }) {
//	};
//};

void mainLoop(GLFWwindow* window, PROGRAM::TYPE type) {
	printf("exiting set up\n\n");
	NETWORK::Network network;
	std::thread networkThread;
	COORDINATOR::Coordinator coordinator;

	switch (type) {
		case PROGRAM::TYPE::OFFLINE:
			break;
		case PROGRAM::TYPE::CLIENT:
			{
				network.initializeClient("localhost", 27015);

				networkThread = std::thread([&network]()
				{
					network.run();
				});
				break;
			}
		case PROGRAM::TYPE::SERVER:
			network.initializeServer(27015);
			networkThread = std::thread([&network]()
			{
				network.run();
			});
			break;
		case PROGRAM::TYPE::HEADLESS_SERVER:
			network.initializeServer(27015);
			networkThread = std::thread([&network]()
			{
				network.run();
			});
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
			client = std::make_unique<Client>(gameState, controlState);
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

	bool graphics = false;
	switch (type) {
		case PROGRAM::TYPE::OFFLINE:
		case PROGRAM::TYPE::CLIENT:
		case PROGRAM::TYPE::SERVER:
			graphics = true;
			break;
		case PROGRAM::TYPE::HEADLESS_SERVER:
			graphics = false;
			break;
		default:
			assert(0);
			break;
	}

	while (true) {
		bool rendering = false;
		if (graphics) {
			if (glfwWindowShouldClose(window)) {
				break;
			}

			if (client->state.uiState.shouldReset()) {
				client->state.uiState.clear();
				client->state.uiState.init();
			}

			{
				if (coordinator.maybeLoadGame) {
					if (type == PROGRAM::TYPE::HEADLESS_SERVER) {
						coordinator.maybeLoadGame.value()->run(gameState, coordinator, std::nullopt);
					}
					else {
						coordinator.maybeLoadGame.value()->run(gameState, coordinator, client.get());
					}

					if (type == PROGRAM::TYPE::SERVER ||
						type == PROGRAM::TYPE::HEADLESS_SERVER) {

						NETWORK::Message message;
						Saver saver(message.buffer);

						coordinator.maybeLoadGame.value()->saveBuffer.seekg(0);

						coordinator.maybeLoadGame.value()->save(saver);

						for (auto& networkClient : network.clients) {
							NETWORK::Message m;
							m.buffer << message.buffer.rdbuf();
							//m.buffer.seekg(message.buffer.tellg());
							//m.buffer.seekp(message.buffer.tellp());

							networkClient->send(std::move(m));
						}
					}

					coordinator.maybeLoadGame = std::nullopt;
				}
			}

			//if (client->state.gameState.loadFile.has_value()) {
			//	auto const& name = client->state.gameState.loadFile.value();
			//	Locator<Log>::ref().putLine("loading: " + name);

			//	std::ifstream save;
			//	Locator<PathManager>::ref().openSave(save, name);

			//	std::unique_ptr<GameLoad> op = std::make_unique<GameLoad>();
			//	op->saveBuffer << save.rdbuf();

			//	//client->state.playerActions.operations.push_back(std::move(op));

			//	std::stringstream e;
			//	Saver s(e);
			//	op->save(s);

			//	Loader l(e);

			//	auto op2 = OPERATION::loadOperation(l);

			//	op2->run(gameState, coordinator);

			//	//Loader(save, client->state.gameState).loadGame();

			//	client->state.player = gameState.getPlayer(0).value();
			//	client->state.gameState.loadFile = std::nullopt;

			//	//coordinator.reset(gameState.tick);
			//}
			//else
			if (client->state.gameState.saveFile.has_value()) {
				auto const& name = client->state.gameState.saveFile.value();
				Locator<Log>::ref().putLine("saving: " + name);

				std::ofstream save;
				Locator<PathManager>::ref().openSave(save, name);

				Saver(save, client->state.gameState).saveGame();
				client->state.gameState.saveFile = std::nullopt;
			}

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
		}


		if (type == PROGRAM::TYPE::OFFLINE ||
			type == PROGRAM::TYPE::CLIENT ||
			type == PROGRAM::TYPE::SERVER) {

			std::lock_guard<std::mutex> lock(coordinator.mutex);

			if (!client->state.playerActions.operations.empty()) {
				coordinator.pushTick(client->state.gameState.tick, std::move(client->state.playerActions));
			}

			for (auto& metaOperation : client->state.metaActions.operations) {
				if (metaOperation->type == COORDINATOR::MESSAGE::TYPE::GAME_LOAD) {
					std::unique_ptr<GameLoad> op = std::unique_ptr<GameLoad>(static_cast<GameLoad*>(metaOperation.release()));
					coordinator.maybeLoadGame = std::move(op);
				}
			}

			client->state.metaActions.operations.clear();
		}
		else if (type == PROGRAM::TYPE::HEADLESS_SERVER) {
			// no client
		}

		if (type == PROGRAM::TYPE::CLIENT ||
			type == PROGRAM::TYPE::SERVER ||
			type == PROGRAM::TYPE::HEADLESS_SERVER) {

			std::lock_guard<std::mutex> lock(network.mutex);
			for (auto& networkClient : network.clients) {
				for (auto& message : networkClient->receivedMessages) {
					coordinator.pushMessage(std::move(message));
					networkClient->receivedMessages.clear();
				}
			}
		}
		else if (type == PROGRAM::TYPE::OFFLINE) {
			// no network
		}

		const static auto gameTick = [&]()
		{
			for (int32_t i = 0; i < 10; i++) {
				if (!tickLimiterGameLogic.ready()) {
					break;
				}
				if (!coordinator.ready(gameState.tick)) {
					break;
				}

				stateChanged = true;

				PlayerActions actions = coordinator.pullTick();

				for (auto& operation : actions.operations) {
					operation->run(gameState, coordinator);
				}

				if (type == PROGRAM::TYPE::SERVER ||
					type == PROGRAM::TYPE::HEADLESS_SERVER) {

					NETWORK::Message message;
					Saver saver(message.buffer);

					actions.save(saver);

					for (auto& networkClient : network.clients) {
						NETWORK::Message m;
						m.buffer << message.buffer.rdbuf();
						m.buffer.seekg(message.buffer.tellg());
						m.buffer.seekp(message.buffer.tellp());

						networkClient->send(std::move(m));
					}
				}

				gameLogic.runStep(client->state.gameState);
				tickLimiterGameLogic.advance();
				Locator<Timer>::ref().endTiming("Game Loop");
				Locator<Timer>::ref().newTiming("Game Loop");
			}
		};

		if (graphics && Option<OPTION::GR_RENDERTHREAD, bool>::getVal()) {
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
