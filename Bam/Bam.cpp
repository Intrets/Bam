#pragma once

#include "common.h"

#include <iostream>

#include "Renderer.h"
#include "CameraInfo.h"
#include "BlockIDTextures.h"
#include "ResourceManagerModel.h"
#include "ResourceManagerTexture.h"
#include "FPSLimiter.h"
#include "GameState.h"
#include "WindowManager.h"

#include <chrono>
#include <thread>
#include "RenderInfo.h"
#include "Option.h"
#include "DebugRenderer.h"
#include "DebugRenderInfo.h"
#include "GameLogic.h"
#include "ControlState.h"
#include "BindHandler.h"

GLFWwindow* window;
ControlState controlState;

static void key_callback(GLFWwindow* w, int key, int scancode, int action, int mods) {
	controlState.key_callback(w, key, scancode, action, mods);
}

static int initGLFW() {
	//glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL 
														 /* Initialize the library */
	if (!glfwInit()) {
		return -1;
	}

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(1000, 1000, "Hello World", NULL, NULL);
	//glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glfwSetWindowPos(window, 500, 20);
	if (!window) {
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glewExperimental = true;

	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	printf("OpenGL version supported by this platform (%s): \n",
		   glGetString(GL_VERSION));

	//glfwSetCharCallback(window, char_callback);
	glfwSetKeyCallback(window, key_callback);
	//glfwSetMouseButtonCallback(window, mouse_callback);

	// constrain cursor to window
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	//glDisable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);
	// back culling
	//glDisable(GL_CULL_FACE);

	glfwSwapInterval(0);

	return 1;
}

int main() {
	initGLFW();

	FPSLimiter fpsLimiter;

	Locator<PathManager>::provide(new PathManager());

	Locator<ResourceManagerModel>::provide(new ResourceManagerModel());
	Locator<ResourceManagerTexture>::provide(new ResourceManagerTexture());

	Locator<OptionManager>::provide(new OptionManager());
	Locator<DebugRenderer>::provide(new DebugRenderer());
	Locator<BlockIDTextures>::provide(new BlockIDTextures());
	Locator<DebugRenderInfo>::provide(new DebugRenderInfo());

	//Locator<ReferenceManager<Activity>>::provide(new ReferenceManager<Activity>());
	//Locator<CommandHandler>::provide(new CommandHandler());

	//Locator<ReferenceManager<BaseWindow>>::provide(new ReferenceManager<BaseWindow>());
	//Locator<ScriptManager>::provide(new ScriptManager());

	Locator<BindHandler>::provide(new BindHandler());

	Locator<BindHandler>::getService()->addBind(
		CONTROLS::LEFT, CONTROLSTATE_DOWN,
		[](GameState& gameState) -> void {
		gameState.playerPos += glm::vec2(-1.0f, 0.0f);
	}
	);
	Locator<BindHandler>::getService()->addBind(
		CONTROLS::RIGHT, CONTROLSTATE_DOWN,
		[](GameState& gameState) -> void {
		gameState.playerPos += glm::vec2(1.0f, 0.0f);
	}
	);
	Locator<BindHandler>::getService()->addBind(
		CONTROLS::UP, CONTROLSTATE_DOWN,
		[](GameState& gameState) -> void {
		gameState.playerPos += glm::vec2(0.0f, 1.0f);
	}
	);
	Locator<BindHandler>::getService()->addBind(
		CONTROLS::DOWN, CONTROLSTATE_DOWN,
		[](GameState& gameState) -> void {
		gameState.playerPos += glm::vec2(0.0f, -1.0f);
	}
	);
	Locator<BindHandler>::getService()->addBind(
		CONTROLS::ZOOM_IN, CONTROLSTATE_DOWN,
		[](GameState& gameState) -> void {
		Option<float> viewportScale{ "viewportscale", 20.0f };
		viewportScale.set(viewportScale * 1.1f);
	}
	);
	Locator<BindHandler>::getService()->addBind(
		CONTROLS::ZOOM_OUT, CONTROLSTATE_DOWN,
		[](GameState& gameState) -> void {
		Option<float> viewportScale{ "viewportscale", 20.0f };
		viewportScale.set(viewportScale / 1.1f);
	}
	);

	WindowManager windowManager;

	GameState gameState;
	GameLogic gameLogic;

	Renderer renderer;

	std::thread logicThread;

	int fps = 0;
	double last = 0;

	while (!glfwWindowShouldClose(window)) {
		//if (gamestate.exit) {
		//	glfwSetWindowShouldClose(window, GLFW_TRUE);
		//	break;
		//}

		RenderInfo renderInfo;

		if (fpsLimiter.ready()) {
			Locator<DebugRenderInfo>::getService()->addPoint(glm::vec2(0));
			renderer.prepareRender(window, renderInfo, gameState, windowManager);
		}

		if (gameLogic.ready()) {
			controlState.cycleStates();
			glfwPollEvents();

			Locator<BindHandler>::getService()->runBinds(controlState, gameState);

			logicThread = std::thread(&GameLogic::runStep, &gameLogic, std::ref(gameState));
		}

		if (fpsLimiter.ready()) {
			if (glfwGetTime() - last > 1.0f) {
				last = glfwGetTime();
				std::cout << fps << "\n";
				fps = 0;
			}
			fps++;

			//fpsdisplay.displayFPS(window);
			fpsLimiter.renderStart();
			renderer.render(window, renderInfo);
			fpsLimiter.renderFinish();
		}

		if (logicThread.joinable()) {
			logicThread.join();
		}

		{
			GLenum err;
			while ((err = glGetError()) != GL_NO_ERROR) {
				std::cout << std::hex << err << std::dec << std::endl;
			}
		}

		std::this_thread::sleep_for(std::chrono::microseconds((long) 1000));
	}
}
