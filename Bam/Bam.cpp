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

GLFWwindow* window;

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
	//glfwSetKeyCallback(window, key_callback);
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

	//Locator<BindHandler>::provide(new BindHandler());

	WindowManager windowManager;

	GameState gameState;
	GameLogic gameLogic;

	Renderer renderer;

	Option<float> viewportScale{ "viewportscale", 20.0f };
	//Option<bool> seperateRenderThread{ "render_thread", true };

	float t = 0;
	std::thread logicThread;

	int fps = 0;
	double last = 0;

	while (!glfwWindowShouldClose(window)) {
		//if (gamestate.exit) {
		//	glfwSetWindowShouldClose(window, GLFW_TRUE);
		//	break;
		//}

		int frameSizeX, frameSizeY;
		glfwGetFramebufferSize(window, &frameSizeX, &frameSizeY);
		float ratio = frameSizeX / static_cast<float>(frameSizeY);
		glm::vec2 viewport(ratio, 1.0f);
		viewport *= viewportScale;

		RenderInfo renderInfo;
		//renderInfo.cameraInfo = { 1000, 1000, glm::vec2{0,0}, glm::vec3(20.0f, 20.0f, 20.0f) };

		//renderInfo.staticWorldRenderInfo.addBlockWithoutShadow(glm::vec2(0, 0), 2);
		//renderInfo.staticWorldRenderInfo.addBlockWithoutShadow(glm::vec2(10, 10), 1);

		//auto camPos = gamestate.cam->getPosition();
		auto camPos = glm::vec2(t, 0);

		renderInfo.cameraInfo = { frameSizeX, frameSizeY, camPos, glm::vec3(viewport, 200.0f) };

		if (fpsLimiter.ready()) {
			Locator<DebugRenderInfo>::getService()->addPoint(glm::vec2(0));
			renderer.prepareRender(renderInfo, gameState, windowManager);
		}

		if (gameLogic.ready()) {
			logicThread = std::thread(&GameLogic::runStep, &gameLogic, std::ref(gameState));
		}

		if (fpsLimiter.ready()) {
			if (glfwGetTime() - last > 1.0f) {
				last = glfwGetTime();
				std::cout << fps << "\n";
				fps = 0;
			}
			fps++;

			//renderer.prepareRender(renderInfo, gamestate, windowManager);
			//renderInfo.debugLines = std::move(Locator<DebugRenderer>::getService()->lines);
			//renderInfo.debugPoints = std::move(Locator<DebugRenderer>::getService()->points);


			//fpsdisplay.displayFPS(window);
			fpsLimiter.renderStart();
			renderer.render(window, renderInfo);
			fpsLimiter.renderFinish();
		}

		if (logicThread.joinable()) {
			logicThread.join();
		}

		//if (physicsHandler.ready() && (!stepByStep || nextStep)) {
		//	Locator<BindHandler>::getService()->runBinds(controlState, gamestate);
		//	nextStep = false;
		//	Locator<DebugRenderer>::getService()->clear();
		//	doneSomething = true;

		//	physicsHandler.runPhysicsTick(controlState, gamestate);

		//	Locator<DebugRenderer>::getService()->points.push_back(glm::vec2(0.0f));

		//	glm::vec3 debugPos = gamestate.player->getPosition();
		//	std::vector<glm::vec2> line;
		//	for (int i = 0; i < 5; i++) {
		//		auto test = (reinterpret_cast<HitBox*>((reinterpret_cast<PhysicsObject*>(gamestate.player)->hitObject.get()))->corners[i % 4]);
		//		line.push_back(test + glm::vec2(debugPos));
		//	}

		//	Locator<DebugRenderer>::getService()->lines.push_back(line);

		//	// TODO: nicely do background
		//	auto temppos = glm::floor(gamestate.player->getPosition());
		//	temppos.z = -100.0f;
		//	background->spatial.origin[background->spatial.currentNode] = temppos;

		//	glm::dvec2 cursorPos;
		//	glfwGetCursorPos(window, &cursorPos.x, &cursorPos.y);
		//	cursorPos.y = frameSizeY - cursorPos.y;
		//	cursorPos.x = -viewport.x + (cursorPos.x / frameSizeX) * 2 * viewport.x;
		//	cursorPos.y = -viewport.y + (cursorPos.y / frameSizeY) * 2 * viewport.y;

		//	gamestate.playerCursor = glm::vec2(gamestate.cam->getPosition()) + glm::vec2(cursorPos);
		//	cursor->setScale(viewportScale / 20.0f);
		//	cursor->spatial.origin[cursor->spatial.currentNode] = glm::vec3(gamestate.playerCursor + (viewportScale / 20.0f) * glm::vec2(0.5f, -0.5f), 10.0f);
		//	controlState.clearJustChanged();
		//}


		{
			GLenum err;
			while ((err = glGetError()) != GL_NO_ERROR) {
				std::cout << std::hex << err << std::dec << std::endl;
			}
		}

		glfwPollEvents();
		//Locator<CommandHandler>::getService()->runCommands(gamestate);

		std::this_thread::sleep_for(std::chrono::microseconds((long) 1000));
	}
}
