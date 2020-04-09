#pragma once

#include "common.h"

#include <iostream>

#include "StaticWorldRenderer.h"
#include "CameraInfo.h"
#include "BlockIDTextures.h"
#include "ResourceManagerModel.h"
#include "ResourceManagerTexture.h"

#include <chrono>
#include <thread>

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

	Locator<PathManager>::provide(new PathManager());

	Locator<ResourceManagerModel>::provide(new ResourceManagerModel());
	Locator<ResourceManagerTexture>::provide(new ResourceManagerTexture());

	//Locator<OptionManager>::provide(new OptionManager());
	//Locator<DebugRenderer>::provide(new DebugRenderer());
	Locator<BlockIDTextures>::provide(new BlockIDTextures());

	//Locator<ReferenceManager<Activity>>::provide(new ReferenceManager<Activity>());
	//Locator<CommandHandler>::provide(new CommandHandler());

	//Locator<ReferenceManager<BaseWindow>>::provide(new ReferenceManager<BaseWindow>());
	//Locator<ScriptManager>::provide(new ScriptManager());

	//Locator<BindHandler>::provide(new BindHandler());

	StaticWorldRenderer test;
	StaticWorldRenderInfo info;
	CameraInfo cameraInfo = { 1000, 1000, glm::vec2{0,0}, glm::vec3(20.0f, 20.0f, 20.0f) };

	info.addBlockWithoutShadow(glm::vec2(0,0), 2);
	info.addBlockWithoutShadow(glm::vec2(10,10), 1);
	while (!glfwWindowShouldClose(window)) {
		if (gamestate.exit) {
			glfwSetWindowShouldClose(window, GLFW_TRUE);
			break;
		}

		int frameSizeX, frameSizeY;
		glfwGetFramebufferSize(window, &frameSizeX, &frameSizeY);
		float ratio = frameSizeX / static_cast<float>(frameSizeY);
		glm::vec2 viewport(ratio, 1.0f);
		viewport *= viewportScale;

		RenderInfo renderInfo;

		auto camPos = gamestate.cam->getPosition();

		renderInfo.cameraInfo = { frameSizeX, frameSizeY, camPos, glm::vec3(viewport, 200.0f) };

		bool doneSomething = false;
		if (fpsLimiter.ready() && !rendering) {
			renderer.prepareRender(renderInfo, gamestate, windowManager);
			renderInfo.debugLines = std::move(Locator<DebugRenderer>::getService()->lines);
			renderInfo.debugPoints = std::move(Locator<DebugRenderer>::getService()->points);

			doneSomething = true;

			fpsdisplay.displayFPS(window);
			fpsLimiter.renderStart();

			if (seperateRenderThread) {
				rendering = true;
				glfwMakeContextCurrent(0);
				rendererThread = std::thread(&Renderer::render, &renderer, window, std::ref(renderInfo), true);
			}
			else {
				renderer.render(window, renderInfo, false);
				fpsLimiter.renderFinish();
			}
		}

		if (physicsHandler.ready() && (!stepByStep || nextStep)) {
			Locator<BindHandler>::getService()->runBinds(controlState, gamestate);
			nextStep = false;
			Locator<DebugRenderer>::getService()->clear();
			doneSomething = true;

			physicsHandler.runPhysicsTick(controlState, gamestate);

			Locator<DebugRenderer>::getService()->points.push_back(glm::vec2(0.0f));

			glm::vec3 debugPos = gamestate.player->getPosition();
			std::vector<glm::vec2> line;
			for (int i = 0; i < 5; i++) {
				auto test = (reinterpret_cast<HitBox*>((reinterpret_cast<PhysicsObject*>(gamestate.player)->hitObject.get()))->corners[i % 4]);
				line.push_back(test + glm::vec2(debugPos));
			}

			Locator<DebugRenderer>::getService()->lines.push_back(line);

			// TODO: nicely do background
			auto temppos = glm::floor(gamestate.player->getPosition());
			temppos.z = -100.0f;
			background->spatial.origin[background->spatial.currentNode] = temppos;

			glm::dvec2 cursorPos;
			glfwGetCursorPos(window, &cursorPos.x, &cursorPos.y);
			cursorPos.y = frameSizeY - cursorPos.y;
			cursorPos.x = -viewport.x + (cursorPos.x / frameSizeX) * 2 * viewport.x;
			cursorPos.y = -viewport.y + (cursorPos.y / frameSizeY) * 2 * viewport.y;

			gamestate.playerCursor = glm::vec2(gamestate.cam->getPosition()) + glm::vec2(cursorPos);
			cursor->setScale(viewportScale / 20.0f);
			cursor->spatial.origin[cursor->spatial.currentNode] = glm::vec3(gamestate.playerCursor + (viewportScale / 20.0f) * glm::vec2(0.5f, -0.5f), 10.0f);
			controlState.clearJustChanged();
		}

		if (rendering) {
			rendererThread.join();
			fpsLimiter.renderFinish();
			glfwMakeContextCurrent(window);
			rendering = false;
		}

		{
			GLenum err;
			while ((err = glGetError()) != GL_NO_ERROR) {
				std::cout << std::hex << err << std::dec << std::endl;
			}
		}

		glfwPollEvents();
		Locator<CommandHandler>::getService()->runCommands(gamestate);

		if (!doneSomething) {
			std::this_thread::sleep_for(std::chrono::microseconds((long) 1000));
		}
	}

	//while (!glfwWindowShouldClose(window)) {
	//	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//	std::cout << "start\n";
	//	test.render(info, 0, cameraInfo);

	//	std::cout << "swap\n";
	//	glfwSwapBuffers(window);
	//	glfwPollEvents();

	//	GLenum err;
	//	while ((err = glGetError()) != GL_NO_ERROR) {
	//		std::cout << std::hex << err << std::dec << std::endl;
	//		// Process/log the error.
	//	}

	//	std::this_thread::sleep_for(std::chrono::milliseconds((long) 16));
	//}
}
